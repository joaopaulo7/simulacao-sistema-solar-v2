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
#include "stdlib.h"
#include "Astro.h"
#include "Camera.h"
#include "SOIL/SOIL.h"
#include <glm/glm.hpp>


#define M 1.9891e30
#define R 7.479893535e9
#define t  (24*60*2)


GLfloat ambient[] = {0.7, 0.7, 0.7, 1.0};
GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat position[] = {0.0, 0.0, 0.0, 1.0};

Astro astros[20];
GLuint idTexturas[20];
int qtdAstros;

GLuint idTexturaSkydome;

int escTempo = 7;
bool luz = true;

void ligaLuz()
{
	if(luz)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);
}

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
}


void atualiza(int escalaTempo){
	for(int i = 0; i < escalaTempo; i++)
	{
		for(int j = 1; j < qtdAstros; j++)
		{
			astros[j].passo();
		}
	}
}

void escreveRastro(Astro a){
	if(luz)
		glDisable(GL_LIGHTING);
	
	glDisable(GL_TEXTURE_2D);
	double ** rastro = a.getRastro();
	int rastroTam = a.getRastroTam();
	int rastroMaxTam = a.getRastroMaxTam(); 
	
	int div = std::max((int)(rastroMaxTam)/50, 1);
	int j = 0;
	int i1 = 0;
	
	glColor3f (.0, .0, .0);
    glBegin(GL_LINE_STRIP);
	for(int i = rastroTam; i < rastroMaxTam + rastroTam; i++)
	{		 
		i1 = i % rastroMaxTam;
		glVertex3f (rastro[i1][0]/R, rastro[i1][1]/R, rastro[i1][2]/R);
		
		if(i % div == 0)
		{
			glEnd();
			glColor4f (1.0, 1.0, 1.0, pow(j/50.0, 2));
			j++;
			glBegin(GL_LINE_STRIP);
			glVertex3f (rastro[i1][0]/R, rastro[i1][1]/R, rastro[i1][2]/R);
		}
	}
    glEnd();
    glEnable(GL_LIGHTING);
    ligaLuz();
    glEnable(GL_TEXTURE_2D);
	   
}

void escreveSkydome(){
	// glPushMatrix();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	
	glBindTexture(GL_TEXTURE_2D, idTexturaSkydome);
	GLUquadric* quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluQuadricNormals(quadObj, GLU_SMOOTH);
	gluQuadricTexture(quadObj, GL_TRUE);
	
	gluSphere(quadObj, 1.5, 16, 20);
	
	gluDeleteQuadric(quadObj);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	// glPopMatrix();
}

void escreveAstro(Astro astro, int i){
	glPushMatrix();
	escreveRastro(astro);
	
	glTranslatef(astro.getPos()[0]/R, astro.getPos()[1]/R, astro.getPos()[2]/R);
	
	glBindTexture(GL_TEXTURE_2D, idTexturas[i]);
	GLUquadric* quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluQuadricNormals(quadObj, GLU_SMOOTH);
	gluQuadricTexture(quadObj, GL_TRUE);
	
	gluSphere(quadObj, astro.getTamanho(), 16, 20);
	
	gluDeleteQuadric(quadObj);
	
	glPopMatrix();
}

void init(void)
{
   glEnable (GL_LINE_SMOOTH);
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
   glLineWidth (1.5);
   
   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
   
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   
   glEnable(GL_AUTO_NORMAL);
   glEnable(GL_NORMALIZE);
  
   
   carregaTexturas();
   glEnable(GL_TEXTURE_2D);
   
}


void display(void)
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	escreveSkydome();

	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
	// glLoadIdentity();
	gluLookAt(astros[3].getPos()[0]*2/R, astros[3].getPos()[1]*2/R, 0.10,
			0.0, 0.0, 0.0,
			0, 0, 1.0);
	
	glLightfv(GL_LIGHT0, GL_POSITION, position);

    ligaLuz();
    GLfloat ambient[] = {1, 1, 1, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    escreveAstro(astros[0], 0);

    GLfloat ambient1[] = {0.3, .3, .3, 1.0};
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
   gluPerspective(45.0f, (float)w/(float)h,  0.1f, 100.0f);

}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
        case 'l':
        case 'L':
			luz = !luz;
			break;
        case 'm':
			astros[0].setM(astros[0].getM() - M);
			break;
        case 'M':
			astros[0].setM(astros[0].getM() + M);
			break;
    }
}

void keyboardEsp(int key, int x, int y)
{
   switch (key) {
        case GLUT_KEY_LEFT: 
			if(escTempo > 3)
				escTempo -= 3;
			break;
        case GLUT_KEY_RIGHT:
			if(escTempo < 728)
				escTempo += 3;
			break;
        case GLUT_KEY_UP: ;  break;
        case GLUT_KEY_DOWN: ;  break;
    }
    
    std::system("clear");
    printf("Escala de tempo: %d dias/s \n", escTempo);
}

void Timer(int unUsed){

	
	atualiza(escTempo);
	
	
    glutPostRedisplay();
    glutTimerFunc(10/3, Timer, 0);
}

// void passiveMouse(int x, int y){
// 	angX += (y - mouseY) / 10.0f;
// 	angY += (x - mouseX) / 10.0f;
// 	mouseX = x;
// 	mouseY = y;
// }

int main(int argc, char** argv)
{
	astros[0].setM(M);
	astros[0].setNomeTex("texturas/2k_sun.jpg");
	
	astros[0].define(0.0, 0.0, 0.0, 0.01, M, &astros[0], "texturas/2k_sun.jpg"); //Sol
	astros[1].define(5.248192e9, 18.766435e7, 75.32, 0.001, 1, &astros[0], "texturas/2k_haumea_fictional.jpg"); //Halley
	astros[2].define(1.51450e9, 1.35255e9, 29.4571, 0.037, 1, &astros[0], "texturas/2k_saturn.jpg"); //Saturno
	astros[3].define(3.00639e9, 2.73556e9, 84.0205, 0.035, 1, &astros[0], "texturas/2k_uranus.jpg"); //Urano
	astros[4].define(8.1662e8, 7.4052e8, 11.862, 0.032, 1, &astros[0], "texturas/2k_jupiter.jpg"); //Jupter
	astros[5].define(7.37593e9, 4.43682e9, 247.94, 0.003, 1, &astros[0], "texturas/2k_pluto.jpg"); //Plutao
	astros[6].define(1.521e8, 1.47095e8, 1, 0.004, 1, &astros[0], "texturas/2k_earth.jpg"); //Terra
	astros[7].define(2.492e8, 2.067e8, 1.88085, 0.004, 1, &astros[0], "texturas/2k_mars.jpg"); //Marte
	
	qtdAstros = 8;
	
    printf("Escala de tempo: %d dias/s \n", escTempo);
	
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize (1080, 1080);
	glutCreateWindow (argv[0]);
	init();
	glutReshapeFunc (reshape);
	glutKeyboardFunc (keyboard);
	// glutMouseFunc(mouseClick);
	// glutMotionFunc(mouseMotion);
	glutSpecialFunc(keyboardEsp);
	glutDisplayFunc (display);
	Timer(0);
	glutMainLoop();
	return 0;
}

