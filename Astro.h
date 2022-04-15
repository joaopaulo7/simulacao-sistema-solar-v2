#ifndef ASTRO_H
#define ASTRO_H

//    the entire header file
class Astro {
    private:
        double pos[3];
        double vel[3];
        
        Astro *pai;
        
        double tamanho;
        double escla;
        double m;
        double periodo;
        
        double **rastro;
        int rastroMaxTam;
        int rastroTam;
        int dia = 0;
        
    public:
        Astro();
        Astro(double aph, double peri, double periodo, double tamanho, double massa, Astro *pai);
        void define(double aph, double peri, double periodo, double tamanho, double massa, Astro *pai);
        void passo();
        void addRastro();
        
        double * getPos();
        double * getVel();
        Astro * getPai();
        double **getRastro();
        double getTamanho();
        double getEscla();
        int getRastroTam();
        int getRastroMaxTam();

        double getM();
        void setM(double mValue);
        double getPeriodo();
};
    
#endif
