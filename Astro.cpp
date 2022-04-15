#include "Astro.h"
#include <math.h>
#include <stdio.h>
#include "stdlib.h"

#define PI 3.1415
#define G 6.674184e-20
#define M 1.9891e30
#define R 7.479893535e9

#define t  (24*60*2)


void Astro::define(double aph, double peri, double periodo, double tamanho, double massa, Astro *pai){
    double a = 0, b = 0, foco = 0, A = 0, c = 0;
	
	this->tamanho = tamanho;
	this->m = massa;
	this->pai = pai;
	
	this->periodo = periodo;
	periodo = periodo*365.244*24*60*60;
	
	
    a = (aph + peri)/2.;
    foco = aph - a;
    b = sqrt(a*a - foco*foco);
    
    A = PI*a*b;
    c = A/periodo;
    
    
    this->vel[0] = -2*c/b;
    this->vel[1] = 0;
    this->vel[2] = 0;
    
    this->pos[0] = foco;
    this->pos[1] = b;
    this->pos[2] = 0;
    
    this->rastroMaxTam = (int)this->periodo*54;
    
	this->rastro = new double*[this->rastroMaxTam];
	for(int i = 0; i < this->rastroMaxTam; i++)
	{
		this->rastro[i] = new double[3];
		rastro[i][0] = foco;
		rastro[i][1] = b;
		rastro[i][2] = 0;
	}
}
Astro::Astro(){
    
}
Astro::Astro(double aph, double peri, double periodo, double tamanho, double massa, Astro *pai){
	define(aph, peri, periodo, tamanho, massa, pai);
}


void Astro::passo(){
    double a[3] = {0, 0, 0};
	double u = 0;
	
	u = pow(sqrt(pow(this->pos[0], 2) + pow(this->pos[1], 2) + pow(this->pos[2], 2)), 3);
	
	double GM = -G*this->pai->m;
	a[0] = GM*this->pos[0]/(u);
	a[1] = GM*this->pos[1]/(u);
	a[2] = GM*this->pos[2]/(u);
	   
	this->vel[0] += a[0]*t;
	this->vel[1] += a[1]*t;
	this->vel[2] += a[2]*t;
	   
	this->pos[0] += this->vel[0]*t;
	this->pos[1] += this->vel[1]*t;
	this->pos[2] += this->vel[2]*t;
	
	if(this->dia == 7*30)
	{
		this->dia = 0;
		this->addRastro();
	}
	this->dia++;
}

void Astro::addRastro(){
    if(this->rastroTam >= this->rastroMaxTam)
		rastroTam = 0;
		
	this->rastro[this->rastroTam][0] = this->pos[0];
	this->rastro[this->rastroTam][1] = this->pos[1];
	this->rastro[this->rastroTam++][2] = this->pos[2];
}

double * Astro::getPos(){
    return this->pos;
}
double * Astro::getVel(){
    return this->vel;
}
Astro * Astro::getPai(){
    return this->pai;
}
double ** Astro::getRastro(){
    return this->rastro;
}
double Astro::getTamanho(){
    return this->tamanho;
}
double Astro::getEscla(){
    return this->escla;
}
int Astro::getRastroTam(){
    return this->rastroTam;
}
int Astro::getRastroMaxTam(){
    return this->rastroMaxTam;
}

double Astro::getM(){
    return this->m;
}
void Astro::setM(double mValue){
    this->m = mValue;
}
double Astro::getPeriodo(){
    return this->periodo;
}
