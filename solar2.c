/*
 * solar.c
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
#include <math.h>

#define PI 3.1415
#define G 6.674184e-20
#define M 1.9891e30
#define R 7.479893535e9

#define t  (24*60*2)

struct astro {
    double pos[3];
    double vel[3];
    
    struct astro *pai;
    
    double tamanho;
    double m;
    double periodo;
};


void definir(double aph, double peri, double periodo, struct astro *p){
	double a = 0, b = 0, foco = 0, A = 0, c = 0;
	
	periodo = periodo*365.244*24*60*60;
	
    a = (aph + peri)/2.;
    foco = aph - a;
    b = sqrt(a*a - foco*foco);
    
    printf("%f\n", b);
    
    A = PI*a*b;
    c = A/periodo;
    
    
    p->vel[0] = -2*c/b;
    p->vel[1] = 0;
    p->vel[2] = 0;
    
    p->pos[0] = foco;
    p->pos[1] = b;
    p->pos[2] = 0;
} 

void passo(struct astro *p){
	double a[3] = {0, 0, 0};
	double u = 0;
	
	u = pow(sqrt(pow(p->pos[0], 2) + pow(p->pos[1], 2) + pow(p->pos[2], 2)), 3);
	
	double GM = -G*p->pai->m;
	a[0] = GM*p->pos[0]/(u);
	a[1] = GM*p->pos[1]/(u);
	a[2] = GM*p->pos[2]/(u);
	   
	p->vel[0] += a[0]*t;
	p->vel[1] += a[1]*t;
	p->vel[2] += a[2]*t;
	   
	p->pos[0] += p->vel[0]*t;
	p->pos[1] += p->vel[1]*t;
	p->pos[2] += p->vel[2]*t;
}

struct astro astros[20];
int qtdAstros = 1;

void atualiza(int escalaTempo){
	for(int i = 0; i < escalaTempo; i++)
	{
		for(int j = 1; j < qtdAstros; j++)
		{
			passo(&astros[j]);
		}
	}
}

void circulo(float escala, double deslocamentoX, double deslocamentoY){
    int n = 30;
    glColor3f (1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < n; i++)
    {
            double y = sin(2*PI/n*i)*escala + deslocamentoY;
            double x = cos(2*PI/n*i)*escala + deslocamentoX;
            glVertex3f ( x, y, 0.0);
    }
    glEnd();
}

void init(void)
{
   glEnable (GL_LINE_SMOOTH);
   glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
   glLineWidth (1.5);
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);   
    glColor3f (1.0, 1.0, 0.0);
    glutSolidSphere(0.02, 10, 4);
    
    /*glColor3f (1.0, 1.0, 1.0);
    glBegin(GL_LINES);
            glVertex3f (0, -1, 0.0);
            glVertex3f (0, 1, 0.0);
            
            glVertex3f (-1, 0, 0.0);
            glVertex3f (1, 0, 0.0);
    glEnd();*/
    
	glColor3f (0.0, .7, .7);
	glPointSize(3);
	for(int i = 1; i < qtdAstros; i++)
	{
		glColor3f (.7, i*1.0/qtdAstros, .5);
		glPushMatrix();
		glTranslatef(astros[i].pos[0]/R, astros[i].pos[1]/R, astros[i].pos[2]/R);
		glutSolidSphere(astros[i].tamanho, 10, 4);    /* draw moon */
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
      case 'r':
      case 'R':
         break;
      case 27:  /*  Escape Key */
         exit(0);
         break;
      default:
         break;
    }
}

void Timer(int unUsed){
	atualiza(500);
    glutPostRedisplay();
    glutTimerFunc(10/3, Timer, 0);
}

int main(int argc, char** argv)
{
	astros[0].m = M;
	astros[1].pai = &astros[0];
	astros[2].pai = &astros[0];
	
	astros[1].tamanho = 0.01;
	astros[2].tamanho = 0.03;
	
	definir(5.248192e9, 18.766435e7, 75.32, &astros[1]);
	definir(1.51450e9, 1.35255e9, 29.4571, &astros[2]);
	
	qtdAstros++;
	qtdAstros++;
	
	printf("%f\n", astros[2].pos[1]);
	printf("%f\n", astros[2].vel[0]);
	
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (1080, 1080);
	glutCreateWindow (argv[0]);
	init();
	glutReshapeFunc (reshape);
	glutKeyboardFunc (keyboard);
	glutDisplayFunc (display);
	Timer(0);
	glutMainLoop();
	return 0;
}

