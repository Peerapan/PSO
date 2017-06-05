#include <math.h>

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

double main_function(int lenx, char* x){
	double y = 0;
	for(int i=0; i<lenx; i++){
		y += x[i]*x[i];
	}
	return y;
}