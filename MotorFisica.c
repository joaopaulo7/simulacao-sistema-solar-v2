#include <math.h>
#include <stdio.h>
#include "stdlib.h"

#define G 6.674184e-20


void copyVec(int dim, double * vec0, double *vec1){
    for(int i = 0; i < dim; i++)
        vec1[i] = vec0[i];
}

void vecSum(int dim, double * vec0, double *vec1){
    for(int i = 0; i < dim; i++)
        vec1[i] += vec0[i];
}

void vecDif(int dim, double * vec0, double *vec1){
    for(int i = 0; i < dim; i++)
        vec1[i] -= vec0[i];
}

double getNorm(int dim, double * vec){
    double norm = .0;
    for(int i = 0; i < dim; i++)
        norm += vec[i]*vec[i];
    return sqrt(norm);
}

void unitVec(int dim, double * vec){     
    double norm = getNorm(dim, vec); 
    for(int i = 0; i < dim; i++)
        vec[i] = vec[i]/norm;
}

void escalarMul(int dim, double * vec, double escalar){
    for(int i = 0; i < dim; i++)
        vec[i] = vec[i]*escalar;
}

void updatePos(int dim, int n_astros, double ** m, double ** pos, double ** vel, int t){
    double aux_GM;
    
    double t_a[dim];
    double aux_a[dim];
    double aux_vel[n_astros][dim];
    
    for(int i = 0; i < n_astros; i++)
    {
               
        escalarMul(dim, t_a, .0);
        for(int j = 0; j < n_astros; j++)
        {
            if(i == j)
                continue;
            
            copyVec(dim, pos[j], aux_a);
            vecDif(dim, pos[i], aux_a);
                
            aux_GM = *m[j]*G/pow(getNorm(dim, aux_a), 2);
            
            unitVec(dim, aux_a);
            escalarMul(dim, aux_a, aux_GM);
            
            vecSum(dim, aux_a, t_a);
        }
        escalarMul(dim, t_a, t);
        vecSum(dim, t_a, vel[i]);
        copyVec(dim, vel[i], aux_vel[i]);
    }


    for(int i = 0; i < n_astros; i++)
    {
        escalarMul(dim, aux_vel[i], t);
        vecSum(dim, aux_vel[i], pos[i]);
    }
}

void nUpdatePos(int dim, int n_astros, double ** m, double ** pos, double ** vel, int t, int n){
    for(int i = 0; i < n; i++)
        updatePos(dim, n_astros, m, pos, vel, t);
}
