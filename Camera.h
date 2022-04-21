// #ifndef CAMERA_H
// #define CAMERA_H

// class Camera {
//    private:
//       double *coo;
//       double *dir;
//       double *cima;
//       double hAngl, vAngl;
      
//    public:
//       Camera(){
//          this->coo = new double[3];
//          this->dir = new double[3];
//          this->cima = new double[3];
//          this->hAngl = 0;
//          this->vAngl = 0;
//       };
//       void setCoo(double x, double y, double z){this->coo[0] = x; this->coo[1] = y; this->coo[2] = z;};
//       void setDir(double x, double y, double z){this->dir[0] = x; this->dir[1] = y; this->dir[2] = z;};
//       void setCima(double x, double y, double z){this->cima[0] = x; this->cima[1] = y; this->cima[2] = z;};
//       void setHAngl(double ang){
//          this->hAngl = ang;
//       }; 
//       void setVAngl(double ang){
//          this->vAngl = ang;
//       };
      
//       // Seta angulação vertical e horizontal da camera por offset do mouse
//       void setCamAngle(int xOffset, int yOffset){
//          this->setVAngl(this->vAngl + yOffset / 100.0f);
// 	      this->setHAngl(this->hAngl + xOffset / 100.0f);
//       };
      
//       void mover(int *coo, int *dir, int *cima);
//       void rotacionar(int xMouse, int yMouse);
// };
    
// #endif