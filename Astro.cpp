#include "Astro.h"
#include <math.h>
#include <stdio.h>
#include "stdlib.h"

#define PI 3.1415
#define G 6.674184e-20
#define M 1.9891e30
#define ANO 365.244*24*60*60

#define t  (60*2)


void Astro::define(double aph, double peri, double periodo, double periodoSyn, double rotacao, double inclinacao, double tamanhoReal, double tamanho, double massa, Astro *pai, std::string nomeTex){
	double a = 0, b = 0, foco = 0, A = 0, c = 0;
		
	this->tamanhoReal = tamanhoReal;
	this->tamanho = tamanho;
	this->m = massa;
	
	this->periodo = periodo;
	this->synVel = 360.0/(periodoSyn*24*60*60.0);
	
	this->pai = pai;
	
	this->nomeTex= nomeTex;

	if(aph > 0.001 and peri > 0.001)
	{		
		periodo = periodo*ANO;
		
		
		a = (aph + peri)/2.;
		foco = aph - a;
		b = sqrt(a*a - foco*foco);
		
		A = PI*a*b;
		c = A/periodo;
		
		
		this->vel[0] = -2*c/b;
		printf("%f\n", -2*c/b);
		this->vel[1] = 0;
		this->vel[2] = 0;
		
		this->pos[0] = foco + this->pai->pos[0];
		this->pos[1] = b + this->pai->pos[1];
		this->pos[2] = 0 + this->pai->pos[2];
		
		this->rastroMaxTam = (int)(this->periodo*52.0);
		
		this->rastro = new double*[this->rastroMaxTam];
		for(int i = 0; i < this->rastroMaxTam; i++)
		{
			this->rastro[i] = new double[3];
			rastro[i][0] = foco + this->pai->pos[0];
			rastro[i][1] = b + this->pai->pos[1];
			rastro[i][2] = 0 + this->pai->pos[2];
		}
	}
	else
	{
		this->vel[0] = 0;
		this->vel[1] = 0;
		this->vel[2] = 0;
		
		this->pos[0] = 0;
		this->pos[1] = 0;
		this->pos[2] = 0;
	}
}
Astro::Astro(){
    
}
Astro::Astro(double aph, double peri, double periodo, double periodoSyn, double rotacao, double inclinacao, double tamanhoReal, double tamanho, double massa, Astro *pai, std::string nomeTex){
	define(aph, peri, periodo, periodoSyn, rotacao, inclinacao, tamanhoReal, tamanho, massa, pai, nomeTex);
}


void Astro::passo(){
    double a[3] = {0, 0, 0};
	double u = 0;
	
	double x = this->pos[0] - this->pai->pos[0];
	double y = this->pos[1] - this->pai->pos[1];
	double z = this->pos[2] - this->pai->pos[2];
	
	u = pow(sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)), 3);
	
	double GM = -G*this->pai->m;
	
	a[0] = GM*(x)/(u);
	a[1] = GM*(y)/(u);
	a[2] = GM*(z)/(u);
	   
	this->vel[0] += a[0]*t;
	this->vel[1] += a[1]*t;
	this->vel[2] += a[2]*t;
	   
	this->pos[0] += (this->vel[0] + this->pai->vel[0])*t;
	this->pos[1] += (this->vel[1] + this->pai->vel[1])*t;
	this->pos[2] += (this->vel[2] + this->pai->vel[2])*t;
	
	if(this->dia > (ANO/t)/52)
	{
		this->dia = 0;
		this->addRastro();
	}
	this->dia++;
	
	this->rotPos = (this->rotPos + this->synVel*t);
	
	if(this->rotPos > 360)
		this->rotPos -= 360;
	else if(this->rotPos < -360)
		this->rotPos += 360;
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
double Astro::getRotPos(){
    return this->rotPos;
}

Astro * Astro::getPai(){
    return this->pai;
}
double ** Astro::getRastro(){
    return this->rastro;
}
double Astro::getTamanhoReal(){
    return this->tamanhoReal;
}
double Astro::getTamanho(){
    return this->tamanho;
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

std::string Astro::getNomeTex(){
    return this->nomeTex;
}
void Astro::setNomeTex(std::string nomeTex){
    this->nomeTex = nomeTex;
}
