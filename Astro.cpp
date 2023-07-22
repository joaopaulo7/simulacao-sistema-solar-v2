#include "Astro.h"
#include <math.h>
#include <stdio.h>
#include "stdlib.h"

#define PI 3.1415
#define G 6.674184e-20 //constante de gravitação universal. Usada para calcular a aceleração.
#define ANO 365.244*24*60*60 //Um ano em segundos (normalizado com bissextos e saltos).


#define SUN 1.9891e30


void Astro::define(int idAstro, double aph, double peri, double periodo, double periodoSyn, double rotacao, double inclinacao, double argumento, double declinacao, double tamanhoReal, double tamanho, double massa, Astro *pai, bool anel, std::string nomeTex){
	//Valores auxiliares da elípse.
	double a = 0, b = 0, foco = 0, A = 0, c = 0;
	
    //Cria vetor de posição e de velocidade 
    double * pos = new double[Astro::dim];
    this->pos = pos;
    Astro::t_pos[idAstro] = pos;
    
    double * vel = new double[Astro::dim];
    this->vel = vel;
    Astro::t_vel[idAstro] = vel;
    
	//Define valores físicos e orbitais do astro
	this->tamanhoReal = tamanhoReal;
	this->tamanho = tamanho;
    
    Astro::t_m[idAstro] = &this->m;
    this->m = massa;
    
	this->declinacao = declinacao - inclinacao;
	
	this->periodo = periodo;
	this->synVel = 360.0/(periodoSyn*24*60*60.0);
	
	this->anel = anel;
	this->nomeTex= nomeTex;
    
	//Se não for o ponto de referência estacionário (Sol)
	if(aph > 0.001 and peri > 0.001)
	{		
		//Graus para radianos
		double al = inclinacao/180*PI;
		double be = -(argumento-90)/180*PI;
		double te = rotacao/180*PI;
		
		
		//calcula matriz de rotação com base nos
		double M[3][3]=
		{{ cos(al)*cos(be)*cos(te) - sin(be)*sin(te), -cos(al)*cos(be)*sin(te) - sin(be)*cos(te),  -sin(al)*cos(be)},
		 { cos(al)*sin(be)*cos(te) + cos(be)*sin(te), -cos(al)*sin(be)*sin(te) + cos(be)*cos(te),  -sin(al)*sin(be)},
		 {                           sin(al)*cos(te), -sin(al)*sin(te)                          ,           cos(al)}};
		
		//anos para segundos
		periodo = periodo*ANO;
		
		//descoberta do foco e eixos da elípse
		a = (aph + peri)/2.;
		foco = aph - a;
		b = sqrt(a*a - foco*foco);
		
		//descoberta da constante de varicação da área
		 //(chamamos de c, mas pode haver um nome mais formal)
		A = PI*a*b;
		c = A/periodo;
		
		//calcula a velocidade incial (devidamente inclinada, usando
		 //a matriz de rotação adequada para os eixos)
		this->vel[0] =  (-2*c/b)*M[0][0] + pai->vel[0];
		this->vel[1] =  (-2*c/b)*M[0][1] + pai->vel[1];
		this->vel[2] =  (-2*c/b)*M[0][2] + pai->vel[2];
		
		
		//calcula a posição incial (devidamente inclinada, usando
		 //a matriz de rotação adequada para os eixos)
		this->pos[0] = (foco)*M[0][0] + b*M[1][0] + pai->pos[0];
		this->pos[1] = (foco)*M[0][1] + b*M[1][1] + pai->pos[1];
		this->pos[2] = (foco)*M[0][2] + b*M[1][2] + pai->pos[2];
		
		//define o tamanho do rastro, baseado no período.
		 //mais lento, mais pontos.
		this->rastroMaxTam = (int)(this->periodo*52.0*2);
		
		//inicializa o rastro
		this->rastro = new double*[this->rastroMaxTam];
		for(int i = 0; i < this->rastroMaxTam; i++)
		{
			this->rastro[i] = new double[3];
			rastro[i][0] = this->pos[0];
			rastro[i][1] = this->pos[1];
			rastro[i][2] = this->pos[2];
		}
	}
	else
	{
		//O ponto de referência estático(Sol) não se move e fica no ponto (0,0,0)
		this->vel[0] = 0;
		this->vel[1] = 0;
		this->vel[2] = 0;
		
		this->pos[0] = 0;
		this->pos[1] = 0;
		this->pos[2] = 0;
        
        this->rastroMaxTam = (int)(2*30.0*2);
		this->rastro = new double*[this->rastroMaxTam];
		for(int i = 0; i < this->rastroMaxTam; i++)
		{
			this->rastro[i] = new double[3];
			rastro[i][0] = this->pos[0];
			rastro[i][1] = this->pos[1];
			rastro[i][2] = this->pos[2];
		}
	}
}

Astro::Astro(){
    
}
Astro::Astro(int idAstro, double aph, double peri, double periodo, double periodoSyn, double rotacao, double inclinacao, double argumento, double declinacao, double tamanhoReal, double tamanho, double massa, Astro *pai,bool anel, std::string nomeTex){
	define(idAstro, aph, peri, periodo, periodoSyn, rotacao, inclinacao, argumento, declinacao, tamanhoReal, tamanho, massa, pai, anel, nomeTex);
}

//alteramos a rotação n vezes
void Astro::updateRot(int n){
	//rotacionamos o astro no prórpio eixo
	this->rotPos = (this->rotPos + this->synVel*Astro::t*n);
	
	//limitamos os valores a <360
	if(this->rotPos > 360)
        this->rotPos = (this->rotPos/360 - trunc(this->rotPos/360))*360;
}

void Astro::addRastro(){
	//adiciona um ponto ao rastro.
	 //vetor com início/fim mutáveis
    if(this->rastroTam >= this->rastroMaxTam)
		rastroTam = 0;
		
	this->rastro[this->rastroTam][0] = this->pos[0];
	this->rastro[this->rastroTam][1] = this->pos[1];
	this->rastro[this->rastroTam++][2] = this->pos[2];
}
