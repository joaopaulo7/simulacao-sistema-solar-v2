/*
 * solar.cpp
 * 
 * Copyright 2022 Joao Paulo Paiva Lima <joao.lima1@estudante.ufla.br>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#include <GL/glut.h>
#include <stdio.h>
#include <cmath>
#include <chrono>
#include "stdlib.h"
#include "Astro.cpp"
#include "Astro.h"
#include "SOIL/SOIL.h"


#define M 1.9891e30
#define R 7.479893535e9

// Configurações de Iluminação
GLfloat ambient[] = {0.7, 0.7, 0.7, 1.0};
GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat position[] = {0.0, 0.0, 0.0, 1.0};

// variaveis referentes ao astro
Astro astros[20];
GLuint idTexturas[20];
int qtdAstros = 0;

// texturas ddo skydome e anel de saturno
GLuint idTexturaSkydome;
GLuint idTexturaAnel;


double vAngl = 0, hAngl = 0; // Angulação vertical e horizontal da camera
int astroIdx = 1; // Variavel de controle de posicionamento da camera
int escTempo = 1; // escala de tempo da simulação
double dist = 10; // zoom da camera

// variaveis controles referentes à escala, iluminação e rotação dos astros
bool luz = true, real = false, rotacao = true; 
char rastros = 1;


void ligaLuz()
{
	if(luz)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);
}

// função utiliza a biblioteca SOIL para carregamento das texturas que serão utilizadas
void carregaTexturas(){
	for(int i = 0; i < qtdAstros; i++)
	{
		printf("%s\n", astros[i].getNomeTex().c_str());
		idTexturas[i] = SOIL_load_OGL_texture(
					  astros[i].getNomeTex().c_str(),
					  SOIL_LOAD_AUTO,
					  SOIL_CREATE_NEW_ID,
					  SOIL_FLAG_INVERT_Y
					  );
	}
		
	idTexturaSkydome = SOIL_load_OGL_texture(
					"texturas/8k_stars_milky_way.jpg",
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					SOIL_FLAG_INVERT_Y
					);
	idTexturaAnel = SOIL_load_OGL_texture(
					"texturas/saturn_ring.png",
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					SOIL_FLAG_INVERT_Y
					);
}

void escreveSkydome(){
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor4f (1.0, 1.0, 1.0, 1.0);
	
	glBindTexture(GL_TEXTURE_2D, idTexturaSkydome);
	GLUquadric* quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluQuadricNormals(quadObj, GLU_SMOOTH);
	gluQuadricTexture(quadObj, GL_TRUE);
	
	//uma esfera texturizada gigante para ser o skydome
	gluSphere(quadObj, 40, 16, 20);

	gluDeleteQuadric(quadObj);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void atualiza(int escalaTempo){
	//Dá n passos com todos os astros.
	Astro::updateEspaco(escalaTempo);
    for(int i = 0; i < qtdAstros; i++)
    {
        astros[i].addRastro();
        astros[i].updateRot(escalaTempo);
    }
}

void escreveRastro(Astro a){
	glPushMatrix();
	
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	
	//variáveis de controle
	double ** rastro = a.getRastro();
	int rastroTam = a.getRastroTam();
	int rastroMaxTam = a.getRastroMaxTam(); 
	
	//divisões para o fade-out
	int div = (int)std::max(rastroMaxTam/50.0, 1.0);
	float divd = floor(rastroMaxTam/div);
	
	int j = 0;
	int i1 = 0;
	
	//escreve o rastro
    glBegin(GL_LINE_STRIP);
	for(int i = rastroTam; i < rastroMaxTam + rastroTam; i++)
	{		 
		i1 = i % rastroMaxTam;
		
		//define a cor dependendo do polo
		if(rastros == 2 and rastro[i1][2]/R > 0)
			glColor4f (1.0, 1, .1, j/(divd));
		else
			glColor4f (1.0, 1.0, 1.0, j/(divd));
		
		//define um vértice
		glVertex3f (rastro[i1][0]/R, rastro[i1][1]/R, rastro[i1][2]/R);
		
		//incrementa o fade-out
		if(i % div == 0)
			j++;
	}
    glEnd();
    
    glEnable(GL_LIGHTING);
    ligaLuz();
    glEnable(GL_TEXTURE_2D);
    
	glPopMatrix();
}

void escreveAnel(Astro astro){
	glPushMatrix();
	
	if(real){
		astro.setRaioInterno(astro.getTamanhoReal() + 0.000003);
		astro.setRaioExterno(astro.getTamanhoReal() + 0.000009);
	} else {
		astro.setRaioInterno(astro.getTamanho() + 4.8e7/R);
		astro.setRaioExterno(astro.getTamanho()+ 1.46e8/R);
	}
	
	glDisable(GL_LIGHTING);
	glColor4f (1.0, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, idTexturaAnel);
	glBegin(GL_QUAD_STRIP);

	float alpha = 0.0f;
	int count = 1;
	while (alpha < 2 * M_PI){
		if (count%2){
			glTexCoord2f(0.0f, 0.0f); glVertex3d(astro.getRaioInterno() * cos(alpha), astro.getRaioInterno() * sin(alpha), 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3d(astro.getRaioExterno() * cos(alpha), astro.getRaioExterno() * sin(alpha), 0.0f);
		} else {
			glTexCoord2f(1.0f, 0.0f); glVertex3d(astro.getRaioInterno() * cos(alpha), astro.getRaioInterno() * sin(alpha), 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3d(astro.getRaioExterno() * cos(alpha), astro.getRaioExterno() * sin(alpha), 0.0f);
		}
		
		alpha += 0.1f;
	}
	if (count%2){
		glTexCoord2f(0.0f, 0.0f); glVertex3d(astro.getRaioInterno() * cos(alpha), astro.getRaioInterno() * sin(alpha), 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3d(astro.getRaioExterno() * cos(alpha), astro.getRaioExterno() * sin(alpha), 0.0f);
	} else {
		glTexCoord2f(1.0f, 0.0f); glVertex3d(astro.getRaioInterno() * cos(alpha), astro.getRaioInterno() * sin(alpha), 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3d(astro.getRaioExterno() * cos(alpha), astro.getRaioExterno() * sin(alpha), 0.0f);
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void escreveAstro(Astro astro, int i){
	glPushMatrix();
	
	//escreve o rastro caso esteja ligado
	if(rastros)
		escreveRastro(astro);
	
	//movimenta para o local correto
	glTranslated(astro.getPos()[0]/R, astro.getPos()[1]/R, astro.getPos()[2]/R);
	
	//inclina e rotaciona
	glRotated(astro.getDeclinacao(), 1.0, 0.0, 0.0);
	if(rotacao)
		glRotated(astro.getRotPos(), 0.0, 0.0, 1.0);
	
	//Cria a esfera
	glBindTexture(GL_TEXTURE_2D, idTexturas[i]);
	GLUquadric* quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluQuadricNormals(quadObj, GLU_SMOOTH);
	gluQuadricTexture(quadObj, GL_TRUE);
	
	//escreve esfera dependendo de qual tamanho tratamos
	if(real)
		gluSphere(quadObj, astro.getTamanhoReal(), 32, 40);
	else
		gluSphere(quadObj, astro.getTamanho(), 32, 40);
	
	gluDeleteQuadric(quadObj);
	
	//escreve anel caso o astro tenha
	if (astro.temAnel()){
		escreveAnel(astro);
	}
	
	glPopMatrix();
}

void init(void)
{
	// Declaração dos Astros
    
    Astro::setEspaco(3, 12);
	astros[qtdAstros].define(qtdAstros++,       0.0,       0.0,             0.0,     25.38,   0, 0,       0,  7.25,  6.96342e5/R,  0.0025,           M, &astros[0], false, "texturas/2k_sun.jpg"); //Sol
	astros[qtdAstros].define(qtdAstros++, 1.52100e8, 1.47095e8,               1,         1,   0, 0, 114.207, 23.43,    6.371e3/R,  0.0025,  5.97237e24, &astros[0], false, "texturas/2k_earth_daymap.jpg");     //Terra
	astros[qtdAstros].define(qtdAstros++, 3.62600e5, 4.06700e5, 0.0748038598854, 29.530589,  -1, 1,       1, 6.687,    1.737e3/R, 0.00243,           1, &astros[1], false, "texturas/2k_moon.jpg");             //Lua
	astros[qtdAstros].define(qtdAstros++,   6.981e7,   4.600e7, 0.24084,       176,  -59.591, 7.005,  29.124,   2.04,  2.439e3/R,   0.001,           1, &astros[0], false, "texturas/2k_mercury.jpg");          //Mercúrio
	astros[qtdAstros].define(qtdAstros++,   1.089e8,   1.074e8,   0.615,   -116.75,   -87.94, 3.394,  54.884,   2.64,  6.051e3/R,   0.002,           1, &astros[0], false, "texturas/2k_venus_atmosphere.jpg"); //Vênus
	astros[qtdAstros].define(qtdAstros++,   2.492e8,   2.067e8, 1.88085,    1.0274,  -60.762, 1.850, 286.502,  25.19,  3.389e3/R,   0.002,           1, &astros[0], false, "texturas/2k_mars.jpg");             //Marte
	astros[qtdAstros].define(qtdAstros++,  8.1662e8,  7.4052e8,  11.862,     0.413, -121.724, 1.303, 273.867,   3.13, 6.9911e4/R,   0.012,           1, &astros[0], false, "texturas/2k_jupiter.jpg");          //Jupter
	astros[qtdAstros].define(qtdAstros++, 1.51450e9, 1.35255e9, 29.4571,     0.439, -124.925, 2.485, 339.392,  26.73, 5.8232e4/R,   0.017,           1, &astros[0], true,  "texturas/2k_saturn.jpg");           //Saturno
	astros[qtdAstros].define(qtdAstros++, 3.00639e9, 2.73556e9, 84.0205,    -0.718,  -85.266, 0.773,  96.998,  97.77, 2.5362e4/R,   0.015,           1, &astros[0], false, "texturas/2k_uranus.jpg");           //Urano
	astros[qtdAstros].define(qtdAstros++, 4.54000e9, 4.46000e9,   164.8,     0.671, -143.043, 1.770, 273.187,  28.32, 2.4622e4/R,   0.014,           1, &astros[0], false, "texturas/2k_neptune.jpg");          //Netuno
	astros[qtdAstros].define(qtdAstros++, 7.37593e9, 4.43682e9,  247.94,    -6.386, -121.559, 17.16, 113.834, 122.53,  2.376e3/R,   0.003,           1, &astros[0], false, "texturas/2k_haumea_fictional.jpg"); //Plutao
	astros[qtdAstros].define(qtdAstros++, 5.24819e9, 8.76643e7,   75.32,   1000000,   -69.68,   162, 110.913,      0,       15/R,   0.001,           2, &astros[0], false, "texturas/2k_haumea_fictional.jpg"); //Halley

	// Configurações de linha
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glLineWidth (2);
	
	// Configurações de luz
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	// Configurações de textura
	carregaTexturas();
	glEnable(GL_TEXTURE_2D);
}

void display(void)
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	
	//Skydome
	escreveSkydome();


	//##MOVIMENTAÇÃO DA CÃMERA##
	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
   	
   	//define variváveis temporárias para melhor leitura
	double tam;
	if(real)
		tam = astros[astroIdx].getTamanhoReal();
	else
		tam = astros[astroIdx].getTamanho();
	double x = astros[astroIdx].getPos()[0];
	double y = astros[astroIdx].getPos()[1];
	double z = astros[astroIdx].getPos()[2];

	//impede inversão brusca nos polos
	if(vAngl > M_PI/2)
		vAngl = M_PI/2 - 0.00001;
	else if(vAngl < -M_PI/2)
		vAngl = -M_PI/2 + 0.00001;
	
	//Usa uma matriz para calcular a posição da câmera.
	gluLookAt((dist*tam)*cos(hAngl)*cos(vAngl) + x/R, (dist*tam)*sin(hAngl)*cos(vAngl) + y/R, dist*tam*sin(vAngl) + z/R,
			x/R, y/R, z/R,
			(dist*tam)*cos(hAngl)*cos(vAngl), (dist*tam)*sin(hAngl)*cos(vAngl), (pow(dist*tam, 2) + pow(2*tam, 2))/(dist*tam));
			
	//##FIM MOVIMENTAÇÃO DA CÃMERA##
	
	//Escreve o sol
	glLightfv(GL_LIGHT0, GL_POSITION, position);
    ligaLuz();
     //sol tem material diferente para brilhar
    GLfloat ambient[] = {1, 1, 1, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    escreveAstro(astros[0], 0);


	//Escreve os outros astros
	 //luz ambiente baixa para efeito de 'noite'
    GLfloat ambient1[] = {0.05, .05, .05, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient1);
    
	for(int i = 1; i < qtdAstros; i++)
	{
		escreveAstro(astros[i], i);
	}
	glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //Já que a escala real é muito menor, precisamos atualizar o Znear.
    if(real)
		gluPerspective(45.0f, (float)w/(float)h,  0.00000001f, 1000.0f);
    else
		gluPerspective(45.0f, (float)w/(float)h,  0.001f, 1000.0f);

}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
        case 'l':
        case 'L':
			luz = !luz;
			break;
		case '\'':
			astroIdx = 0;
			hAngl = 0;
			break;
		case '1':
			astroIdx = 1;
			hAngl = 0;
			break;
		case '2':
			astroIdx = 2;
			hAngl = 0;
			break;
		case '3':
			astroIdx = 3;
			hAngl = 0;
			break;
		case '4':
			astroIdx = 4;
			hAngl = 0;
			break;
		case '5':
			astroIdx = 5;
			hAngl = 0;
			break;
		case '6':
			astroIdx = 6;
			hAngl = 0;
			break;
		case '7':
			astroIdx = 7;
			hAngl = 0;
			break;
		case '8':
			astroIdx = 8;
			hAngl = 0;
			break;
		case '9':
			astroIdx = 9;
			hAngl = 0;
			break;
		case '0':
			astroIdx = 10;
			hAngl = 0;
			break;
		case '-':
			astroIdx = 11;
			hAngl = 0;
			break;
		case 't':
			rastros = (rastros + 1)%3;
			break;
		case 'r':
			rotacao = !rotacao;
			break;
        case 'R':
			real = !real;
			//atualizamos o zNear
			reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			break;
        case 'm':
			astros[0].setM(astros[0].getM() - M);
			break;
        case 'M':
			astros[0].setM(astros[0].getM() + M);
			break;
    }
}

//aumentar e diminuir a escala de tempo.
void keyboardEsp(int key, int x, int y)
{
	int add = 1;
	if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
		add = 120;
   switch (key) {
        case GLUT_KEY_LEFT: 
			if(escTempo > add)
				escTempo -= add;
			else
				escTempo = 0;
			break;
        case GLUT_KEY_RIGHT:
			if(escTempo < 2*365*24)
				escTempo += add;
			else if( escTempo == 0)
				escTempo = 1;
			break;
        case GLUT_KEY_UP: ;  break;
        case GLUT_KEY_DOWN: ;  break;
    }
    
    std::system("clear");
    printf("Escala de tempo: %d horas/s \n", escTempo);
}

void Timer(int unUsed){
	//Usa a escala de tempo para os valores da simuação 
	 //mantendo 30fps. A simulação independe do display.
	auto t_start = std::chrono::high_resolution_clock::now();
	
	atualiza(escTempo);
    glutPostRedisplay();
    
    //correção do fps com relação ao tempo de processamento da simulação
    auto t_end = std::chrono::high_resolution_clock::now();
    double dif = std::chrono::duration<double, std::milli>(t_end-t_start).count();
    
    //atualiza 30 por segundo 1000/30 milissegundos de espera.
    glutTimerFunc(std::max(1000.0/30 - dif, 1.0), Timer, 0);
}

// posicoes iniciais do mouse
GLfloat mouseX = 0.0f;
GLfloat mouseY = 0.0f; 


//movimento do mouse
void mouseMotionCallback(int x, int y){

	vAngl += (y - mouseY) / 100.0f;
	hAngl += (x - mouseX) / 100.0f;
	mouseX = x;
	mouseY = y;

}

void mouseClickCallback(int button, int state, int x, int y) {
	float add = .5;
	if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
		add = 10;
	
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseX = x;
			mouseY = y;
		}
	}
	else if(button == 3 and dist > 2 + add)
		dist -= add;
	else if(button == 4)
		dist += add;
}


int main(int argc, char** argv)
{	
    printf("Escala de tempo: %d horas/s \n", escTempo);
	
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize (1080, 1080);
	glutCreateWindow (argv[0]);
	init();
	glutReshapeFunc (reshape);
	glutKeyboardFunc (keyboard);
	glutMouseFunc(mouseClickCallback);
	glutMotionFunc(mouseMotionCallback);
	glutSpecialFunc(keyboardEsp);
	glutDisplayFunc (display);
	Timer(0);
	glutMainLoop();
	return 0;
}

