#include <math.h>
#include <limits>

double sigmoid(double x)
{
	double exp_value;
	double return_value;
	
	/*** Exponential calculation ***/
	exp_value = exp((double) -x);
	
	/*** Final sigmoid value ***/
	return_value = 1 / (1 + exp_value);
	
	return return_value;
}

double logsig(double n){
	return 1 / (1 + exp(-n));
}

int mybin2dec(int l, char* x){
	int y=0;
	if(x==0){
		y=0;
	}
	else{
		y=0;
		for(int i=0;i<=l-1;i++){
			y=y+x[i+1]*pow(2,i);
		}
	}
	return y;
}

int sign(double x){
	return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

void minimum(int& index, double& min_val, int fx_size, double* fx){
	min_val = std::numeric_limits<double>::max();
	index = -1;
	for(int i=0;i<fx_size;i++){
		if( fx[i] < min_val ){
			min_val = fx[i];
			index = i;
		}
	}
}

double fx_function1(int x_size, char* x){
	double y = 0;
	for(int i=0;i<x_size;i++){
		if(x[i]){
			y += x[i];
		}
	}
	return y;
}

double fx_function2(int x_size, char* x){
	double y = 0;
	for(int i=0; i<x_size; i++){
		y += x[i]*x[i];
	}
	return y;
}