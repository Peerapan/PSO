#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <limits>

#include "function.h"

#define WEIGHT 1000

int side_of_working, ss, ls, import_container, free_area, export_container, movement;
double **time_side_to_a;
double **time_side_to_c;
double **time_a_to_c;
int* upper;
int* lower;
int* start_bit;
int* bitnum;

int decimal_2_binary_size(int input){
	int i = 1;
	while( input > 1 ){
		input >>= 1;
		i++;
	}
	return i;
}

int binary_2_decimal(int bsize, char* bits){
	int j = 1;
	int sum = 0;
	for(int i=0;i<bsize;i++){
		sum += (j * bits[i]);
		j <<= 1;
	}
	return sum;
}

double fx_function_solve(int x_size, char* x, bool display){
	int max_size = ls+ss;
	int mss = ss;
	int mls = ls+ss;
	int ma = free_area;
	int set_export_container[export_container];
	int set_area[free_area];
	for(int i=0;i<export_container;i++)
		set_export_container[i] = i;
	for(int i=0;i<free_area;i++)
		set_area[i] = i;
	double y = 0;
	for(int i=0;i<ss;i++){
		int ic = binary_2_decimal(bitnum[i*2], &x[start_bit[i*2]]);
		int ia = binary_2_decimal(bitnum[i*2+1], &x[start_bit[i*2+1]]);
		ic = (ic / pow(2,bitnum[i*2])) * upper[i*2];
		ia = (ia / pow(2,bitnum[i*2+1])) * upper[i*2+1];
		int tc = set_export_container[ic];
		int ta = set_area[ia];
		set_export_container[ic] = set_export_container[mss-1];
		set_area[ia] = set_area[ma-1];
		mss--;
		ma--;
		double sum = time_side_to_a[0][ta] + time_a_to_c[ta][tc] + time_side_to_c[0][tc];
		if(display) printf("SS: 1, %d, %d, %d, %d, %lf(%lf, %lf, %lf)\n", i+1, ta+1, tc+1, i+1, sum, time_side_to_a[0][ta], time_a_to_c[ta][tc], time_side_to_c[0][tc]);
		y += sum;
	}
	for(int i=ss;i<(ls+ss);i++){
		int ic = binary_2_decimal(bitnum[i*2], &x[start_bit[i*2]]);
		int ia = binary_2_decimal(bitnum[i*2+1], &x[start_bit[i*2+1]]);
		ic = (ic / pow(2,bitnum[i*2])) * upper[i*2];
		ia = (ia / pow(2,bitnum[i*2+1])) * upper[i*2+1];
		ic += ss;
		int tc = set_export_container[ic];
		int ta = set_area[ia];
		set_export_container[ic] = set_export_container[mls-1];
		set_area[ia] = set_area[ma-1];
		mls--;
		ma--;
		double sum = time_side_to_a[1][ta] + time_a_to_c[ta][tc] + time_side_to_c[1][tc];
		if(display) printf("LS: 2, %d, %d, %d, %d, %lf(%lf, %lf, %lf)\n", i+1, ta+1, tc+1, i+1, sum, time_side_to_a[1][ta], time_a_to_c[ta][tc], time_side_to_c[1][tc]);
		y += sum;
	}
	return y;
}

int calculate_malloc_size(){
	int sbit = 0;
	for(int i=0;i<ss; i++){
		lower[i*2] = 0;
		lower[i*2+1] = 0;
		upper[i*2] = ss-1-i;
		upper[i*2+1] = free_area-1-i;
		start_bit[i*2] = sbit;
		bitnum[i*2] = decimal_2_binary_size(upper[i*2]);
		sbit += bitnum[i*2];
		start_bit[i*2+1] = sbit;
		bitnum[i*2+1] = decimal_2_binary_size(upper[i*2+1]);
		sbit += bitnum[i*2+1];
/*		printf("bit %d(%d:%d), %d(%d:%d)\n",
			   start_bit[i*2], upper[i*2], bitnum[i*2],
			   start_bit[i*2+1], upper[i*2+1], bitnum[i*2+1]); */
	}
	for(int j=0;j<ls; j++){
		int i = j+ss;
		lower[i*2] = 0;
		lower[i*2+1] = 0;
		upper[i*2] = ls-1-j;
		upper[i*2+1] = free_area-1-j-ss;
		start_bit[i*2] = sbit;
		bitnum[i*2] = decimal_2_binary_size(upper[i*2]);
		sbit += bitnum[i*2];
		start_bit[i*2+1] = sbit;
		bitnum[i*2+1] = decimal_2_binary_size(upper[i*2+1]);
		sbit += bitnum[i*2+1];
/*		printf("bit %d(%d:%d), %d(%d:%d)\n",
			   start_bit[i*2], upper[i*2], bitnum[i*2],
			   start_bit[i*2+1], upper[i*2+1], bitnum[i*2+1]); */
	}
//	printf("count: %d\n", sbit);
	return sbit;
}

void read_data(const char* file){
	FILE *ptr = NULL;
	ptr = fopen(file,"r");
	if(ptr){
		fscanf(ptr, "%d %d %d %d %d %d %d", &side_of_working, &ss, &ls, &import_container, &free_area, &export_container, &movement );
		time_side_to_a = (double**) malloc(sizeof(double*) * side_of_working);
		for(int i=0; i<side_of_working; i++){
			time_side_to_a[i] = (double*) malloc(sizeof(double) * free_area);
			for(int j=0; j<free_area; j++){
				fscanf(ptr, "%lf", &time_side_to_a[i][j]);
//				printf("%lf ", time_side_to_a[i][j]);
			}
//			printf("\n");
		}
		time_side_to_c = (double**) malloc(sizeof(double*) * side_of_working);
		for(int i=0; i<side_of_working; i++){
			time_side_to_c[i] = (double*) malloc(sizeof(double) * export_container);
			for(int j=0; j<export_container; j++){
				fscanf(ptr, "%lf", &time_side_to_c[i][j]);
//				printf("%lf\n", time_side_to_c[i][j]);
			}
		}
		time_a_to_c = (double**) malloc(sizeof(double*) * free_area);
		for(int i=0; i<free_area; i++){
			time_a_to_c[i] = (double*) malloc(sizeof(double) * export_container);
			for(int j=0; j<export_container; j++){
				fscanf(ptr, "%lf", &time_a_to_c[i][j]);
//				printf("%lf ", time_a_to_c[i][j]);
			}
//			printf("\n");
		}
	}
	fclose(ptr);
}

int init(char*& input){
	read_data(input);
	upper = (int*) malloc(sizeof(int)*((ss+ls)*2));
	lower = (int*) malloc(sizeof(int)*((ss+ls)*2));
	start_bit = (int*) malloc(sizeof(int)*((ss+ls)*2));
	bitnum = (int*) malloc(sizeof(int)*((ss+ls)*2));
	int allocate_size = calculate_malloc_size();
	
	return allocate_size;
}

void uninit(){
	free(upper);
	free(lower);
	free(start_bit);
	free(bitnum);
	for(int i=0; i<side_of_working; i++){
		free(time_side_to_a[i]);
	}
	free(time_side_to_a);
	for(int i=0; i<side_of_working; i++){
		free(time_side_to_c[i]);
	}
	free(time_side_to_c);
	for(int i=0; i<free_area; i++){
		free(time_a_to_c[i]);
	}
	free(time_a_to_c);
}

int main (int argc, char** argv){
	int malloc_size = init(argv[1]);
	
//	printf("BIT SIZE: %d\n", malloc_size);
	
	double Pbest1=std::numeric_limits<double>::max();
	double Gbest1=std::numeric_limits<double>::max();
	int popsize = 1000;
	char** x = (char**)malloc(sizeof(char*)*popsize);
	char** xpbest = (char **)malloc(sizeof(char*)*popsize);
	double** vel = (double**)malloc(sizeof(double*)*popsize);
	double** one_vel = (double**)malloc(sizeof(double*)*popsize);
	double** zero_vel = (double**)malloc(sizeof(double*)*popsize);
	for(int i=0;i<popsize;i++){
		x[i] = (char*)malloc(sizeof(char)*malloc_size);
		xpbest[i] = (char*)malloc(sizeof(char)*malloc_size);
		vel[i] = (double*)malloc(sizeof(double)*malloc_size);
		one_vel[i] = (double*)malloc(sizeof(double)*malloc_size);
		zero_vel[i] = (double*)malloc(sizeof(double)*malloc_size);
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
			pbest[i] = fx[i] = fx_function_solve(malloc_size, x[i], false);
		}
		
		double w1 = 0.5;
		double c1 = 1;
		double c2 = 1;
		int maxiter = 100;
		double vmax = 4;
		
		int l;
		double gbest;
		double gg;
		
		minimum(l, gbest, popsize, fx);
		memcpy(xgbest, x[l], malloc_size);
		
		for(int iter=1;iter<=maxiter;iter++){
			double w = 0.5;
			for(int i=0;i<popsize;i++){
				fx[i] = fx_function_solve(malloc_size, x[i], false);
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
			
			double c3 = c1*((double) rand() / (RAND_MAX));
			double dd3 = c2*((double) rand() / (RAND_MAX));
			
			for(int i=0;i<popsize;i++){
				for(int j=0;j<malloc_size;j++){
					double oneadd = 0, zeroadd = 0;
					if(xpbest[i][j] == 0){
						oneadd = oneadd-c3;
						zeroadd = zeroadd+c3;
					}else{
						oneadd = oneadd+c3;
						zeroadd = zeroadd-c3;
					}
					if(xgbest[j] == 0){
						oneadd = oneadd-dd3;
						zeroadd = zeroadd+dd3;
					}else{
						oneadd = oneadd+dd3;
						zeroadd = zeroadd-dd3;
					}
					one_vel[i][j]=(w1*one_vel[i][j])+oneadd;
					zero_vel[i][j]=(w1*zero_vel[i][j])+zeroadd;
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
		}
//		printf("%lf %lf\n", Gbest1, Pbest1);
	}
	printf("%s : %lf\n", argv[1], Gbest1);
	fx_function_solve(malloc_size, xgbest, true);
	
	for(int i=0;i<popsize;i++){
		free(x[i]);
		free(xpbest[i]);
		free(vel[i]);
		free(one_vel[i]);
		free(zero_vel[i]);
	}
	free(x);
	free(xpbest);
	free(vel);
	free(one_vel);
	free(zero_vel);
	free(fx);
	free(pbest);
	free(xgbest);
	
	return 0;
}