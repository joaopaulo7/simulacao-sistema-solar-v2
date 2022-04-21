#include "Astro.h"
#include <math.h>
#include <stdio.h>
#include "stdlib.h"

#define PI 3.1415
#define G 6.674184e-20 //constante de gravitação universal. Usada para calcular a aceleração.
#define ANO 365.244*24*60*60 //Um ano em segundos (normalizado com bissextos e saltos).

#define t  (60) //escala de 60 passos por hora dentro da simulação.


void Astro::define(double aph, double peri, double periodo, double periodoSyn, double rotacao, double inclinacao, double argumento, double declinacao, double tamanhoReal, double tamanho, double massa, Astro *pai, bool anel, std::string nomeTex){
	//Valores auxiliares da elípse.
	double a = 0, b = 0, foco = 0, A = 0, c = 0;
	
	//Define valores físicos e orbitais do astro
	this->tamanhoReal = tamanhoReal;
	this->tamanho = tamanho;
	this->m = massa;
	this->declinacao = declinacao - inclinacao;
	
	this->periodo = periodo;
	this->synVel = 360.0/(periodoSyn*24*60*60.0);
	
	this->pai = pai;
	
	this->anel = anel;
	this->nomeTex= nomeTex;

	//Se não for o ponto de referência estacionário (Sol)
	if(aph > 0.001 and peri > 0.001)
	{		
		//Graus para radianos
		double al = inclinacao/180*PI;
		double be = (argumento-114.207)/180*PI;
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
		this->vel[0] =  (-2*c/b)*M[0][0];
		this->vel[1] =  (-2*c/b)*M[0][1];
		this->vel[2] =  (-2*c/b)*M[0][2];
		
		
		//calcula a posição incial (devidamente inclinada, usando
		 //a matriz de rotação adequada para os eixos)
		this->pos[0] = (foco)*M[0][0] + b*M[1][0] + this->pai->pos[0];
		this->pos[1] = (foco)*M[0][1] + b*M[1][1] + this->pai->pos[1];
		this->pos[2] = (foco)*M[0][2] + b*M[1][2] + this->pai->pos[2];
		
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
	}
}

Astro::Astro(){
    
}
Astro::Astro(double aph, double peri, double periodo, double periodoSyn, double rotacao, double inclinacao, double argumento, double declinacao, double tamanhoReal, double tamanho, double massa, Astro *pai,bool anel, std::string nomeTex){
	define(aph, peri, periodo, periodoSyn, rotacao, inclinacao, argumento, declinacao, tamanhoReal, tamanho, massa, pai, anel, nomeTex);
}

//calculamos os valores de posição e velocidade no momento t+1
void Astro::passo(){
    double a[3] = {0, 0, 0};
	double u = 0;
	if(this->periodo != 0)
	{
		//variáveis auxiliares para melhor leitura.
		double x = this->pos[0] - this->pai->pos[0];
		double y = this->pos[1] - this->pai->pos[1];
		double z = this->pos[2] - this->pai->pos[2];
		
		//calculamos a norma do vetor unitário gravidade e multiplicamos por r^2.
		 //essa multiplicação acontece pois multiplicamos o vetor g
		 //pelo dividendo do escalar calculado na lei da gravitação de Newton.
		u = pow(sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)), 3);
		
		//calcula o escalar e multiplica pelo vetor unitario da gravidade.
		 //para encontrarmos o vetor gravidade com a devida norma.
		double GM = -G*this->pai->m;
		a[0] = GM*(x)/(u);
		a[1] = GM*(y)/(u);
		a[2] = GM*(z)/(u);
		  
		//passo na velocidade (aceleração)
		this->vel[0] += a[0]*t;
		this->vel[1] += a[1]*t;
		this->vel[2] += a[2]*t;
		
		//passo na posição (velocidade)
		this->pos[0] += (this->vel[0] + this->pai->vel[0])*t;
		this->pos[1] += (this->vel[1] + this->pai->vel[1])*t;
		this->pos[2] += (this->vel[2] + this->pai->vel[2])*t;
		
		//salvamos algumas posições para a escrita do rastro.
		if(this->dia > (ANO/(t))*this->periodo/this->rastroMaxTam)
		{
			this->dia = 0;
			this->addRastro();
		}
		this->dia++;
	}
	
	//rotacionamos o astro no prórpio eixo
	this->rotPos = (this->rotPos + this->synVel*t);
	
	//limitamos os valores a +-360
	if(this->rotPos > 360)
		this->rotPos -= 360;
	else if(this->rotPos < -360)
		this->rotPos += 360;
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


//##GETS E SETS
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
double Astro::getM(){
    return this->m;
}
double Astro::getDeclinacao(){
    return this->declinacao;
}

int Astro::getRastroTam(){
    return this->rastroTam;
}
int Astro::getRastroMaxTam(){
    return this->rastroMaxTam;
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
