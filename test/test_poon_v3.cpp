#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <limits>
#include <set>
#include <vector>
#include <string>

#include "function.h"

#define WEIGHT 1000

struct dat{
    int _h;
    int _w;
    int _l;
    dat(int x, int y, int z) : _h(x), _w(y), _l(z) { }
    bool operator<(const dat& b) const {
        if (_h != b._h) return _h < b._h;
        else if (_w != b._w) return _w < b._w;
        else return _l < b._l;
    }
};

int W, H, L;
std::set<dat*> containers;
std::set<dat*> areas;
std::vector<dat*> cc_containers;
int imp_ss, imp_ls;
std::vector<int> exp_ss;
std::vector<int> exp_ls;
std::vector<std::string> all_commands;

std::vector<int> res;

int** table;

int* upper;
int* lower;
int* start_bit;
int* bitnum;

double fx_function_solve(int x_size, char* x, bool display) {
    double y = 0;
    return y;
}

int calculate_malloc_size() {
    int sbit = 0;
    
    return sbit;
}

void read_data(const char* file) {
    FILE *ptr = NULL;
    ptr = fopen(file, "r");
    if (ptr) {
        int n;
        int x, y, z;
        fscanf(ptr, "%d %d %d", &H, &W, &L);
        
        //! malloc table
        table = (int**)malloc(sizeof(int*) * W);
        for(int i=0;i<W;i++){
            table[i] = (int*)malloc(sizeof(int) * L);
            memset(table[i], 0, L);
        }
        
        fscanf(ptr, "%d", &n);
        for(int i=0; i<n; i++){
            fscanf(ptr, "%d %d %d", &x, &y, &z);
            dat* _a = new dat(x, y, z);
            containers.insert(_a);
            dat* _b = new dat(x, y, z);
            cc_containers.push_back(_b);
            if( x > table[y][z]){
                table[y][z] = x;
            }
        }
        fscanf(ptr, "%d", &n);
        for(int i=0; i<n; i++){
            fscanf(ptr, "%d %d %d", &x, &y, &z);
            dat* _a = new dat(x, y, z);
            areas.insert(_a);
        }
        fscanf(ptr, "%d", &imp_ss);
//        fscanf(ptr, "%d", &imp_ls);
        fscanf(ptr, "%d", &n);
        int t;
        for(int i=0; i<n; i++){
            fscanf(ptr, "%d", &t);
            exp_ss.push_back(t);
            dat* _t = cc_containers[i];
        }
//        fscanf(ptr, "%d", &n);
//        for(int i=0; i<n; i++){
//            fscanf(ptr, "%d", &t);
//            exp_ss.push_back(t);
//        }
    }
//    for(auto& it : containers){
//        printf("%d %d %d\n", it->_h, it->_w, it->_l);
//    }
    fclose(ptr);
}

void analyze(){
    
}

int init(char*& input) {
    read_data(input);
    analyze();
    int allocate_size = calculate_malloc_size();
    return allocate_size;
}

void uninit() {

}

int main(int argc, char** argv) {
    int malloc_size = init(argv[1]);

    //	printf("BIT SIZE: %d\n", malloc_size);

    double Pbest1 = std::numeric_limits<double>::max();
    double Gbest1 = std::numeric_limits<double>::max();
    int popsize = 1000;
    char** x = (char**) malloc(sizeof (char*)*popsize);
    char** xpbest = (char **) malloc(sizeof (char*)*popsize);
    double** vel = (double**) malloc(sizeof (double*)*popsize);
    double** one_vel = (double**) malloc(sizeof (double*)*popsize);
    double** zero_vel = (double**) malloc(sizeof (double*)*popsize);
    for (int i = 0; i < popsize; i++) {
        x[i] = (char*) malloc(sizeof (char)*malloc_size);
        xpbest[i] = (char*) malloc(sizeof (char)*malloc_size);
        vel[i] = (double*) malloc(sizeof (double)*malloc_size);
        one_vel[i] = (double*) malloc(sizeof (double)*malloc_size);
        zero_vel[i] = (double*) malloc(sizeof (double)*malloc_size);
    }
    double* fx = (double *) malloc(sizeof (double)*popsize);
    double* pbest = (double *) malloc(sizeof (double)*popsize);
    char* xgbest = (char *) malloc(sizeof (char)*malloc_size);

    srand(time(0));
    for (int tt = 0; tt < 10; tt++) {
        for (int i = 0; i < popsize; i++) {
            for (int j = 0; j < malloc_size; j++) {
                xpbest[i][j] = x[i][j] = rand() % 2;
                vel[i][j] = ((double) rand() / (RAND_MAX)) - 0.5;
                one_vel[i][j] = ((double) rand() / (RAND_MAX)) - 0.5;
                zero_vel[i][j] = ((double) rand() / (RAND_MAX)) - 0.5;
            }
        }

        for (int i = 0; i < popsize; i++) {
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

        for (int iter = 1; iter <= maxiter; iter++) {
            double w = 0.5;
            for (int i = 0; i < popsize; i++) {
                fx[i] = fx_function_solve(malloc_size, x[i], false);
                if (fx[i] < pbest[i]) {
                    pbest[i] = fx[i];
                    memcpy(xpbest[i], x[i], malloc_size);
                }
            }
            minimum(l, gg, popsize, fx);
            if (gbest > gg) {
                gbest = gg;
                memcpy(xgbest, x[l], malloc_size);
            }

            double c3 = c1 * ((double) rand() / (RAND_MAX));
            double dd3 = c2 * ((double) rand() / (RAND_MAX));

            for (int i = 0; i < popsize; i++) {
                for (int j = 0; j < malloc_size; j++) {
                    double oneadd = 0, zeroadd = 0;
                    if (xpbest[i][j] == 0) {
                        oneadd = oneadd - c3;
                        zeroadd = zeroadd + c3;
                    } else {
                        oneadd = oneadd + c3;
                        zeroadd = zeroadd - c3;
                    }
                    if (xgbest[j] == 0) {
                        oneadd = oneadd - dd3;
                        zeroadd = zeroadd + dd3;
                    } else {
                        oneadd = oneadd + dd3;
                        zeroadd = zeroadd - dd3;
                    }
                    one_vel[i][j] = (w1 * one_vel[i][j]) + oneadd;
                    zero_vel[i][j] = (w1 * zero_vel[i][j]) + zeroadd;
                    if (fabs(vel[i][j]) > vmax) {
                        zero_vel[i][j] = vmax * sign(zero_vel[i][j]);
                        one_vel[i][j] = vmax * sign(one_vel[i][j]);
                    }
                    if (x[i][j] == 1) {
                        vel[i][j] = zero_vel[i][j];
                    } else {
                        vel[i][j] = one_vel[i][j];
                    }
                    if (((double) rand() / (RAND_MAX)) < logsig(vel[i][j])) {
                        x[i][j] = !x[i][j];
                    } else {
                        x[i][j] = x[i][j];
                    }
                }
            }
        }
        if (Gbest1 > gbest) {
            Gbest1 = gbest;
        }
        double temp = 0;
        for (int i = 0; i < popsize; i++) {
            temp += pbest[i];
        }
        temp /= popsize;
        if (Pbest1 > temp) {
            Pbest1 = temp;
        }
        //		printf("%lf %lf\n", Gbest1, Pbest1);
    }
    printf("%s : %lf\n", argv[1], Gbest1);
    fx_function_solve(malloc_size, xgbest, true);

    for (int i = 0; i < popsize; i++) {
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