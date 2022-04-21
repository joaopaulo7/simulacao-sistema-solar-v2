#ifndef ASTRO_H
#define ASTRO_H
#include <string>

//    the entire header file
class Astro {
    private:
		//valores vetoriais e de rotação
        double pos[3];
        double vel[3];
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
        Astro();
        Astro(double aph, double peri, double periodo, double periodoSyn, double rotacao, double inclinacao, double declinacao, double tamanhoReal, double tamanho, double massa, Astro *pai, bool anel, std::string nomeTex);
        void define(double aph, double peri, double periodo, double periodoSyn, double rotacao, double inclinacao, double declinacao, double tamanhoReal,  double tamanho, double massa, Astro *pai, bool anel, std::string nomeTex);
        
        void passo();

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
    
#endif
