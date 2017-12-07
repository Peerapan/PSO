#ifndef FUNCTION_H
#define FUNCTION_H

double sigmoid(double x);
double logsig(double n);
int sign(double x);
void minimum(int& index, double& min_val, int fx_size, double* fx);

int decimal_2_binary_size(int input);
int binary_2_decimal(int bsize, char* bits);

int adjust(int curr, int max_curr, int max_n);

#endif