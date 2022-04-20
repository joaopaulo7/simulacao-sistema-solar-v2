#ifndef ASTRO_H
#define ASTRO_H
#include <string>

//    the entire header file
class Astro {
    private:
        double pos[3];
        double vel[3];
        double rotPos;
        
        Astro *pai;
        
        double tamanho;
        double tamanhoReal;
        double m;
        double declinacao;
        
        double periodo;
        double synVel;
        
        double **rastro;
        int rastroMaxTam;
        int rastroTam;
        int dia = 0;

        double raioInterno = 0;
        double raioExterno = 0;
        
        std::string nomeTex;
        bool anel;
        
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
