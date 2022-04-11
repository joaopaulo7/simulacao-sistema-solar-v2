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
#define R 2.479893535e9

#define t  (7*24*60*2)

struct astro {
    double x;
    double y;
    
    struct astro *pai;
    
    double m;
    double a;
    double b;
    double foco;
    double A;
    double periodo;
};

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

void definir(double aph, double peri, struct astro p){
      p.a = (aph + peri)/2;
      p.foco = aph - p.a;
      p.b = (p.a + 2* sqrt(pow(p.a, 2) - 3*pow(p.foco, 2)))/3; //Baskara
      p.A = PI*p.a*p.b;
  } 

void init(void)
{
   glEnable (GL_LINE_SMOOTH);
   glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
   glLineWidth (1.5);
}
double x = 8.097733e7, y = 1.428929e9;
double v[2] = {-9.684, 0};
double a[2] = {0, 0};
double u = 0;
int count = 0;
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   //  double x = 8.097733e7, y = 1.428929e9;
   //  double v[2] = {-9.684, 0};
   //  double a[2] = {0, 0};
   //  double u = 0;
   
    
    circulo(0.01, -1.3525e9/R, 0); 
    
    circulo(0.01, 8.097733e7/R,  1.428929e9/R);
    circulo(0.01, 8.097733e7/R,  -1.428929e9/R);
     
    circulo(0.01, 1.5145e9/R, 0);
    glColor3f (1.0, 1.0, 1.0);
    glBegin(GL_LINES);
            glVertex3f (0, -1, 0.0);
            glVertex3f (0, 1, 0.0);
            
            glVertex3f (-1, 0, 0.0);
            glVertex3f (1, 0, 0.0);
    glEnd();
    
   glColor3f (1.0, 0.0, 0.0);
   glPointSize(3);
   glPushMatrix();
   glTranslatef(x/R, y/R, 0.0);
   // glBegin(GL_POINTS);
   //  for(int i = 0; i < 30*54*30; i++)
   //  {
      // glVertex3f (x/R, y/R, 0.0);
      
      // glVertex3f (0.0, 0.0, 0.0);
   // glTranslatef(x/R, y/R, 0.0);
   glutSolidSphere(0.1, 10, 4);    /* draw moon */
   //  }
   // glEnd();
   glPopMatrix();
   glFlush();

   u = pow(sqrt(x*x + y*y), 3);
   a[0] = -G*M*x/(u);
   a[1] = -G*M*y/(u);
   
   v[0] += a[0]*t;
   v[1] += a[1]*t;
   
   x += v[0]*t;
   y += v[1]*t;
   
   if(y  > -1e6 && y < 1e6)
   {
      printf("%f %f\n", x, y);
   }
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
   glutPostRedisplay();
   glutTimerFunc(1, Timer, 0);
}

int main(int argc, char** argv)
{
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

