#ifndef CAMERA_H
#define CAMERA_H

class Camera {
   private:
      int coo[3];
      int dir[3];
      int cima[3];
      
   public:
      Camera(int *coo, int *dir, int *cima);
      void mover(int *coo, int *dir, int *cima);
      void rotacionar(int xMouse, int yMouse);
};
    
#endif