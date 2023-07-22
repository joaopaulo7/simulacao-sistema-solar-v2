#include <math.h>
#include <stdio.h>
#include "stdlib.h"
#include <pthread.h>

#define G 6.674184e-20

pthread_barrier_t barreira;
const int N_THREADS = 8;
int dim = 3;

void copyVec(double * vec0, double *vec1){
    for(int i = 0; i < dim; i++)
        vec1[i] = vec0[i];
}

void vecSum(double * vec0, double *vec1){
    for(int i = 0; i < dim; i++)
        vec1[i] += vec0[i];
}

void vecDif(double * vec0, double *vec1){
    for(int i = 0; i < dim; i++)
        vec1[i] -= vec0[i];
}

double getNorm(double * vec){
    double norm = .0;
    for(int i = 0; i < dim; i++)
        norm += vec[i]*vec[i];
    return sqrt(norm);
}

void unitVec(double * vec){     
    double norm = getNorm(vec); 
    for(int i = 0; i < dim; i++)
        vec[i] = vec[i]/norm;
}

void escalarMul(double escalar, double * vec){
    for(int i = 0; i < dim; i++)
        vec[i] = vec[i]*escalar;
}


void escalaESum(double escalar, double * vec0, double *vec1){
    escalarMul(escalar, vec0);
    vecSum(vec0, vec1);
}


int n_astros;
double ** pos;
double ** vel;
double ** m;
long unsigned int delta_m;
int t;


void * updatePos(void * thread_id){
    int i_thread = *(int *) thread_id;
    
    double aux_GM;
    double t_a[dim];
    double aux_a[dim];
    double aux_vel[n_astros][dim];
    
    int i_ini = floor(float(i_thread    )/N_THREADS * n_astros);
    int i_fim = floor(float(i_thread + 1)/N_THREADS * n_astros);
    
    
    for(long unsigned k = 0; k < delta_m; k++)
    {
        for(int i = i_ini; i < i_fim; i++)
        {
            escalarMul(0.0, t_a);
            for(int j = 0; j < n_astros; j++)
            {
                if(i == j)
                    continue;
                
                copyVec(pos[j], aux_a);
                vecDif(pos[i], aux_a);
                    
                aux_GM = *m[j]*G/pow(getNorm(aux_a), 2);
                
                unitVec(aux_a);
                
                escalaESum(aux_GM, aux_a, t_a);
            }
            escalaESum(t, t_a, vel[i]);
            copyVec(vel[i], aux_vel[i]);
        }
        //pthread_barrier_wait(&barreira);
        for(int i = i_ini; i < i_fim; i++)
            escalaESum(t, aux_vel[i], pos[i]);
        //pthread_barrier_wait(&barreira);
    }
    return NULL;
}


void nUpdatePos(int c_n_astros, double ** c_m, double ** c_pos, double ** c_vel, int c_t, int c_delta_m){
    n_astros = c_n_astros;
    m = c_m;
    pos =  c_pos;
    vel =  c_vel;
    t =  c_t;
    delta_m = c_delta_m;
    
    int barrados = N_THREADS;
    int thread_ids[N_THREADS];
    pthread_t threads[N_THREADS];
    
    pthread_barrier_init(&barreira, NULL, barrados);
    for(int thread_id = 0; thread_id < N_THREADS; thread_id++)
    {
        thread_ids[thread_id] = thread_id;
        pthread_create(&threads[thread_id], NULL, updatePos, &thread_ids[thread_id]);
    }
    for(int thread_id = 0; thread_id < N_THREADS; thread_id++)
        pthread_join(threads[thread_id], NULL);
        
    pthread_barrier_destroy(&barreira);
}
