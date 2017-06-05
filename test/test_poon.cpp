#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "function.h"

int main (int argv, char** argc){
	double Pbest1=10^65;
	double Gbest1=10^65;
	int vec = 3, N = 3;
	int popsize = 100;
	int bit = 20;
	char* ones = (char *)malloc(sizeof(char)*bit);
	char* zeros = (char *)malloc(sizeof(char)*bit);
	memset(ones,1,bit);
	memset(zeros,0,bit);
	int up = 50;
	int low = -50;
	int upb = mybin2dec(bit, ones);
	int lowb = mybin2dec(bit, zeros);
	int malloc_size = bit*vec;
	
	char** x = (char**)malloc(sizeof(char*)*popsize);
	double** vel = (double**)malloc(sizeof(double*)*popsize);
	double** one_vel = (double**)malloc(sizeof(double*)*popsize);
	double** zero_vel = (double**)malloc(sizeof(double*)*popsize);
	for(int i=0;i<popsize;i++){
		x[i] = (char*)malloc(sizeof(char)*malloc_size);
		vel[i] = (double*)malloc(sizeof(double)*malloc_size);
		one_vel[i] = (double*)malloc(sizeof(double)*malloc_size);
		zero_vel[i] = (double*)malloc(sizeof(double)*malloc_size);
	}
	double* fx = (double *)malloc(sizeof(double)*popsize);
	
	srand(time(0));
	for(int tt=0;tt<10;tt++){
		for(int i=0;i<popsize;i++){
			for(int j=0;j<malloc_size;j++){
				x[i][j] = rand()%2;
				vel[i][j] = ((double) rand() / (RAND_MAX))-0.5;
				one_vel[i][j] = ((double) rand() / (RAND_MAX))-0.5;
				zero_vel[i][j] = ((double) rand() / (RAND_MAX))-0.5;
			}
		}
		
		/*
		double* pbest;
		char* xpbest;
		for(int i=0; i<popsize; i+=1){
			char* xn = (char *)malloc(sizeof(char)*N);
			for(int j=0; j<N; j+=1){
				
			}
//			fx[i] = main_function(lenxn, xn);
		}
		pbest = fx;
//		xpbest = x;
		double w1 = 0.5;
		double c1 = 1;
		double c2 = 1;
		int maxiter = 1000;
		double vmax = 4;
		
		for(int iter=1;iter<=maxiter;iter++){
//			double w = (maxiter-iter)/maxiter;
			double w = 0.5;
			for(int i=0;i<popsize;i++){
				
			}
			
			char** oneadd = (char**)malloc(sizeof(char*)*popsize);
			char** zeroadd = (char**)malloc(sizeof(char*)*popsize);
			for(int i=0;i<popsize;i+=1){
				oneadd[i] = (char*)malloc(sizeof(char)*malloc_size);
				zeroadd[i] = (char*)malloc(sizeof(char)*malloc_size);
				memset(oneadd,0,malloc_size);
				memset(zeroadd,0,malloc_size);
			}
			
			for(int i=0;i<popsize;i++){
				for(int j=0;j<malloc_size;j++){
					if(xpbest[i][j] == 0){
						
					}else{
					
					}
					if(xgbest[j] == 0){
						
					}else{
						
					}
				}
			}
			
			for()
			
		}
		
		double gbest, pbest;
		if(Gbest1 > gbest){
			Gbest1 = gbest;
		}
		double temp = sum(pbest)/popsize;
		if(Pbest1>temp){
			Pbest1 = temp;
		}
		*/
	}
	free(ones);
	free(zeros);
	for(int i=0;i<popsize;i++){
		free(x[i]);
		free(vel[i]);
		free(one_vel[i]);
		free(zero_vel[i]);
	}
	free(x);
	free(vel);
	free(one_vel);
	free(zero_vel);
	free(fx);
	return 0;
}