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
#include "stdlib.h"
#include <cmath>
#include "Astro.cpp"
#include "Camera.h"

#define PI 3.1415
#define G 6.674184e-20
#define M 1.9891e30
#define R 7.479893535e9

#define t  (24*60*2)

Astro astros[20];
int qtdAstros = 1;

int escTempo = 7;

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
				 glColor3f (j/50.0, j/50.0, j/50.0);
				 j++;
				 glBegin(GL_LINE_STRIP);
				 glVertex3f (rastro[i1][0]/R, rastro[i1][1]/R, rastro[i1][2]/R);
			 }
		 }
     glEnd();
}

void init(void)
{
   glEnable (GL_LINE_SMOOTH);
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
   glLineWidth (1.5);
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);   
    glColor3f (1.0, 1.0, 0.0);
    glutSolidSphere(0.01, 10, 4);
    
	glColor3f (0.0, .7, .7);
	glPointSize(3);
	for(int i = 1; i < qtdAstros; i++)
	{
		escreveRastro(astros[i]);
		glColor3f (.7, i*1.0/qtdAstros, .5);
		glPushMatrix();
		glTranslatef(astros[i].getPos()[0]/R, astros[i].getPos()[1]/R, astros[i].getPos()[2]/R);
		glutSolidSphere(astros[i].getTamanho(), 20, 16);    /* draw moon */
		glPopMatrix();
	}
	glFlush();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h) 
      gluOrtho2D (-1.0, 1.0, 
         -1.0*(GLfloat)h/(GLfloat)w, 1.0*(GLfloat)h/(GLfloat)w);
   else 
      gluOrtho2D (-1.0*(GLfloat)w/(GLfloat)h, 
         1.0*(GLfloat)w/(GLfloat)h, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
        case GLUT_KEY_LEFT:
			break;
        case GLUT_KEY_RIGHT:
			break;
        case GLUT_KEY_UP: ;  break;
        case GLUT_KEY_DOWN: ;  break;
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

int main(int argc, char** argv)
{
	astros[0].setM(M);
	
	astros[1].define(5.248192e9, 18.766435e7, 75.32, 0.002, 1, &astros[0]); //Halley
	astros[2].define(1.51450e9, 1.35255e9, 29.4571, 0.006, 1, &astros[0]); //Saturno
	astros[3].define(3.00639e9, 2.73556e9, 84.0205, 0.006, 1, &astros[0]); //Urano
	astros[4].define(8.1662e8, 7.4052e8, 11.862, 0.006, 1, &astros[0]); //Jupter
	astros[5].define(7.37593e9, 4.43682e9, 247.94, 0.003, 1, &astros[0]); //Plutao
	astros[6].define(1.521e8, 1.47095e8, 1, 0.004, 1, &astros[0]); //Terra
	astros[7].define(2.492e8, 2.067e8, 1.88085, 0.004, 1, &astros[0]); //Marte
	
	
	qtdAstros = 8;
	
    printf("Escala de tempo: %d dias/s \n", escTempo);
	
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (1080, 1080);
	glutCreateWindow (argv[0]);
	init();
	glutReshapeFunc (reshape);
	glutKeyboardFunc (keyboard);
	glutSpecialFunc(keyboardEsp);
	glutDisplayFunc (display);
	Timer(0);
	glutMainLoop();
	return 0;
}

