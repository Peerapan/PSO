#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <limits>

#include "function.h"

int main (int argv, char** argc){
	double Pbest1=std::numeric_limits<double>::max();
	double Gbest1=std::numeric_limits<double>::max();
	int vec = 1, N = 1;
	int popsize = 100;
	int bit = 10000;
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
	char** xpbest = (char **)malloc(sizeof(char*)*popsize);
	double** vel = (double**)malloc(sizeof(double*)*popsize);
	double** one_vel = (double**)malloc(sizeof(double*)*popsize);
	double** zero_vel = (double**)malloc(sizeof(double*)*popsize);
	double** oneadd = (double**)malloc(sizeof(double*)*popsize);
	double** zeroadd = (double**)malloc(sizeof(double*)*popsize);
	for(int i=0;i<popsize;i++){
		x[i] = (char*)malloc(sizeof(char)*malloc_size);
		xpbest[i] = (char*)malloc(sizeof(char)*malloc_size);
		vel[i] = (double*)malloc(sizeof(double)*malloc_size);
		one_vel[i] = (double*)malloc(sizeof(double)*malloc_size);
		zero_vel[i] = (double*)malloc(sizeof(double)*malloc_size);
		oneadd[i] = (double*)malloc(sizeof(double)*malloc_size);
		zeroadd[i] = (double*)malloc(sizeof(double)*malloc_size);
		
	}
	double* fx = (double *)malloc(sizeof(double)*popsize);
	double* pbest = (double *)malloc(sizeof(double)*popsize);
	char* xgbest = (char *)malloc(sizeof(char)*malloc_size);
	
	srand(time(0));
	for(int tt=0;tt<10;tt++){
		for(int i=0;i<popsize;i++){
			for(int j=0;j<malloc_size;j++){
				xpbest[i][j] = x[i][j] = rand()%2;
				vel[i][j] = ((double) rand() / (RAND_MAX))-0.5;
				one_vel[i][j] = ((double) rand() / (RAND_MAX))-0.5;
				zero_vel[i][j] = ((double) rand() / (RAND_MAX))-0.5;
			}
		}
		
		for(int i=0;i<popsize;i++){
			pbest[i] = fx[i] = fx_function1(malloc_size, x[i]);
		}
		
		double w1 = 0.5;
		double c1 = 1;
		double c2 = 1;
		int maxiter = 1000;
		double vmax = 4;
		
		int l;
		double gbest;
		double gg;
		
		minimum(l, gbest, popsize, fx);
		memcpy(xgbest, x[l], malloc_size);
		
		for(int iter=1;iter<=maxiter;iter++){
			
			double w = 0.5;
			for(int i=0;i<popsize;i++){
				fx[i] = fx_function1(malloc_size, x[i]);
				if( fx[i] < pbest[i] ){
					pbest[i] = fx[i];
					memcpy( xpbest[i], x[i], malloc_size);
				}
			}
			minimum(l, gg, popsize, fx);
			if( gbest > gg ){
				gbest = gg;
				memcpy(xgbest, x[l], malloc_size);
			}
			
			
			for(int i=0;i<popsize;i+=1){
				for(int j=0;j<popsize;j+=1){
					oneadd[i][j] = 0;
					zeroadd[i][j] = 0;
				}
			}
			
			double c3 = c1*rand();
			double dd3 = c2*rand();
			
			for(int i=0;i<popsize;i++){
				for(int j=0;j<malloc_size;j++){
					if(xpbest[i][j] == 0){
						oneadd[i][j] = oneadd[i][j]-c3;
						zeroadd[i][j] = zeroadd[i][j]+c3;
					}else{
						oneadd[i][j] = oneadd[i][j]+c3;
						zeroadd[i][j] = zeroadd[i][j]-c3;
					}
					if(xgbest[j] == 0){
						oneadd[i][j] = oneadd[i][j]-dd3;
						zeroadd[i][j] = zeroadd[i][j]+dd3;
					}else{
						oneadd[i][j] = oneadd[i][j]+dd3;
						zeroadd[i][j] = zeroadd[i][j]-dd3;
					}
				}
			}
			
			for(int i=0;i<popsize;i++){
				for(int j=0;j<malloc_size;j++){
					one_vel[i][j]=w1*one_vel[i][j]+oneadd[i][j];
					zero_vel[i][j]=w1*zero_vel[i][j]+zeroadd[i][j];
					if( fabs(vel[i][j]) > vmax ){
						zero_vel[i][j] = vmax * sign(zero_vel[i][j]);
						one_vel[i][j] = vmax * sign(one_vel[i][j]);
					}
					if(x[i][j] == 1){
						vel[i][j]=zero_vel[i][j];
					}else{
						vel[i][j]=one_vel[i][j];
					}
					if(((double) rand() / (RAND_MAX)) < logsig(vel[i][j])){
						x[i][j] = !x[i][j];
					}else{
						x[i][j] = x[i][j];
					}
				}
			}
			
			if( iter % 10 == 0)
				printf("TT(%d), ITER(%d) : %lf\n", tt, iter, gbest);
		}
		if(Gbest1 > gbest){
			Gbest1 = gbest;
		}
		double temp = 0;
		for(int i=0;i<popsize;i++){
			temp += pbest[i];
		}
		temp /= popsize;
		if(Pbest1>temp){
			Pbest1 = temp;
		};
		printf("%lf %lf\n", Gbest1, Pbest1);
	}
	
	free(ones);
	free(zeros);
	for(int i=0;i<popsize;i++){
		free(x[i]);
		free(xpbest[i]);
		free(vel[i]);
		free(one_vel[i]);
		free(zero_vel[i]);
		free(oneadd[i]);
		free(zeroadd[i]);
	}
	free(x);
	free(xpbest);
	free(vel);
	free(one_vel);
	free(zero_vel);
	free(oneadd);
	free(zeroadd);
	free(fx);
	free(pbest);
	free(xgbest);
	return 0;
}