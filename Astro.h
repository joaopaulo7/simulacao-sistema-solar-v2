#ifndef ASTRO_H
#define ASTRO_H
#include <string>
#include "MotorFisica.c"

//    the entire header file
class Astro {
    private:
        
        //dimensão e número de astros
        static const int t = (60*2);
        static int dim;
        static int n_astros;
        
        //valores vetoriais e de rotação de todos os astros
        
        static double ** t_vel;
        static double ** t_m;
                
		//valores vetoriais e de rotação de um objeto
        double *pos;
        double *vel;
        double rotPos;
        
        //astro a aqual orbita
        Astro *pai;
        
        //características físicas
        double tamanho;
        double tamanhoReal;
        double m;
        double declinacao;
        
        //Período (tempo até uma volta en torno do sol)
        double periodo;
        
        //velocidade de rotação sinótica (no próprio eixo)
        double synVel;
        
        //valores para o rastro
        double **rastro;
        int rastroMaxTam;
        int rastroTam;
        int dia = 0;
		
		//valores para o anel
        double raioInterno = 0;
        double raioExterno = 0;
        bool anel;
        
        //textura
        std::string nomeTex;
        
    public:
        static double ** t_pos;
        static void setEspaco(int dim, int n_astros);
        static void updateEspaco(int n);
        
        Astro();
        Astro(int idAstro, double aph, double peri, double periodo, double periodoSyn, double rotacao, double inclinacao, double argumento, double declinacao, double tamanhoReal, double tamanho, double massa, Astro *pai, bool anel, std::string nomeTex);
        void define(int idAstro, double aph, double peri, double periodo, double periodoSyn, double rotacao, double inclinacao, double argumento, double declinacao, double tamanhoReal,  double tamanho, double massa, Astro *pai, bool anel, std::string nomeTex);
        
        void updateRot(int t);

        void setRaioInterno(double raio){this->raioInterno = raio;};
        void setRaioExterno(double raio){this->raioExterno = raio;};
        
        double getRaioInterno(){return this->raioInterno;};
        double getRaioExterno(){return this->raioExterno;};

        bool temAnel(){return this->anel;};
        double * getPos();
        double * getVel();
        double getRotPos();
        
        Astro * getPai();
        
        double getTamanhoReal();
        double getTamanho();
        double getDeclinacao();
        
        void addRastro();
        int getRastroTam();
        int getRastroMaxTam();
        double **getRastro();

        double getM();
        void setM(double mValue);
        
        double getPeriodo();
        
        std::string getNomeTex();
        void setNomeTex(std::string nomeTex);
};

//dimensão e número de astros
int Astro::dim;
int Astro::n_astros;
        
//valores vetoriais e de rotação de todos os astros
double ** Astro::t_pos;
double ** Astro::t_vel;

//valores de massa de todos os astros
double ** Astro::t_m;
                

void Astro::updateEspaco(int n){
    nUpdatePos(Astro::dim, Astro::n_astros, Astro::t_m, Astro::t_pos, Astro::t_vel, Astro::t, n);
}


//##GETS E SETS
void Astro::setEspaco(int dim, int n_astros){
    Astro::dim = dim;
    Astro::n_astros = n_astros;
    
    Astro::t_pos = new double* [n_astros];
    Astro::t_vel = new double* [n_astros];
    Astro::t_m =   new double* [n_astros];
    
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


#endif
