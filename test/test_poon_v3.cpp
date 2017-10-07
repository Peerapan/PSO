#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <map>
#include <set>
#include <vector>
#include <string>
#include <stack>
#include <limits>
#include <iostream>
#include <algorithm>

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

int imp_ss, imp_ls;
std::set<int> exp_ss;
std::set<int> exp_ls;
std::set<int> res;

std::map<int, dat*> cc_containers;
std::map<int, dat*> areas;
std::vector< std::vector< std::vector<int> > > table;

int* upper;
int* lower;
int* start_bit;
int* bitnum;

int res_steps;
int imp_ss_steps;
int exp_ss_steps;
int res_bits;
int ss_bits;

std::vector<int> pool;

void reset(){

}

void init_res_pool(){
    for(int i=0;i<W;i++){
        for(int j=0;j<L;j++){
            int k = table[i][j].size()-1;
            if(k > 0 && res.find(table[i][j][k]) != res.end()){
                pool.push_back( table[i][j][k] );
            }
        }
    }
}

void init_exp_pool(){
    for(int i=0;i<W;i++){
        for(int j=0;j<L;j++){
            int k = table[i][j].size()-1;
            if(k > 0 && exp_ss.find(table[i][j][k]) != res.end()){
                pool.push_back( table[i][j][k] );
            }
        }
    }
}

int adjust(int curr, int max_curr, int max_n){
    return (curr / max_curr ) * max_n;
}

int pop_pool(std::vector<int>& pool, int idx){
    int r = pool[idx];
    pool[idx] = pool[pool.size()-1];
    pool.pop_back();
    return r;
}

double fx_function_solve(int x_size, char* x, bool display) {
    double y = 0;
    reset();
    init_res_pool();
    int start = 0;
    int all = W*L;
    int res_bit = decimal_2_binary_size(res_steps);
    int all_bit = decimal_2_binary_size(all);
    for(int i=0; i<res_steps; i++){
        int res_it = binary_2_decimal(res_bit, x+start);
        start += res_bit;
        int area_it = binary_2_decimal(all_bit, x+start);
        start += all_bit;
        int idx_r = adjust(res_it,all-1,pool.size()-1);
        int r = pop_pool(pool, idx_r);
        int des = adjust(area_it,all-1,areas.size()-1);
        int _x = areas[des]->_h;
        int _y = areas[des]->_w;
        int _z = areas[des]->_l;
        if(display){
            printf("Move %d( %d, %d, %d ) to ( %d, %d, %d )\n",r ,
                cc_containers[r]->_h, cc_containers[r]->_w, cc_containers[r]->_l,
                areas[des]->_h , areas[des]->_w, areas[des]->_l);
        }
        y+= abs(cc_containers[r]->_l - areas[des]->_l);
        //adjust
//        table[areas[des]->_w][areas[des]->_l].push_back(idx_r);
//        table[cc_containers[r]->_w][cc_containers[r]->_l].pop_back();
//        cc_containers[r]->_h = areas[des]->_h;
//        cc_containers[r]->_w = areas[des]->_w;
//        cc_containers[r]->_l = areas[des]->_l;
//        areas.erase(des);
    }
    pool.clear();
    for(int i=0; i<imp_ss_steps+exp_ss_steps; i++){
        
    }
    return y;
}

int calculate_malloc_size() {
    int sbit = 0;
    res_steps = res.size();
    res_bits = (decimal_2_binary_size(res_steps) + decimal_2_binary_size(W*L) * res_steps);
    printf("RES BITS: %d\n", res_bits);
    imp_ss_steps = imp_ss;
    exp_ss_steps = exp_ss.size();
    int max_ss_steps = std::max(imp_ss_steps, exp_ss_steps);
    int total_ss_steps = imp_ss_steps + exp_ss_steps;
    ss_bits = (1 + decimal_2_binary_size(total_ss_steps) + decimal_2_binary_size(W*L)) * (total_ss_steps);
    printf("SS BITS: %d\n", ss_bits);
    sbit = res_bits + ss_bits;
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
        table.resize(W);
        for(int i=0;i<W;i++){
            table[i].resize(L);
        }
        
        fscanf(ptr, "%d", &n);
        for(int i=0; i<n; i++){
            fscanf(ptr, "%d %d %d", &x, &y, &z);
            dat* _a = new dat(x, y, z);
            cc_containers.insert(std::make_pair(i, _a));
        }
        fscanf(ptr, "%d", &n);
        for(int i=0; i<n; i++){
            fscanf(ptr, "%d %d %d", &x, &y, &z);
            dat* _a = new dat(x, y, z);
            areas.insert(std::make_pair(i, _a));
        }
        fscanf(ptr, "%d", &imp_ss);
//        fscanf(ptr, "%d", &imp_ls);
        fscanf(ptr, "%d", &n);
        int t;
        for(int i=0; i<n; i++){
            fscanf(ptr, "%d", &t);
            exp_ss.insert(t);
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
    std::vector<std::pair<int, dat*> > pairs;
    for (auto itr = cc_containers.begin(); itr != cc_containers.end(); ++itr)
        pairs.push_back(*itr);
    std::sort(pairs.begin(), pairs.end(), [=](std::pair<int, dat*>& a, std::pair<int, dat*>& b)
    {
        return a.second->_h < b.second->_h;
    }
    );
    for(auto& it : pairs){
        table[it.second->_w][it.second->_l].push_back(it.first);
    } 
    for(int i=0;i<W;i++){
        for(int j=0;j<L;j++){
            for(int k=0;k<table[i][j].size();k++){
                if(exp_ss.find(table[i][j][k]) != exp_ss.end()){
                    for(int l=k+1; l<table[i][j].size(); l++){
                        res.insert(table[i][j][l]);
                    }
                    break;
                }
            }
        }
    }
    //For Debug
    for(auto& it: res){
        std::cout << it << std::endl;
    }
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
//    int malloc_size = init(argv[1]);
    char* file_name = "./data/example_data.txt";
    int malloc_size = init(file_name);
    
    printf("BIT SIZE: %d\n", malloc_size);

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
    printf("%s : %lf\n", file_name, Gbest1);
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