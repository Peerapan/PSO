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

//#define DEBUG

#define WEIGHT 1000

#define TRAVEL_TIME 2
#define CONTROL_TIME 10

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

std::map<int, dat*> cc_containers;
std::map<int, dat*> areas;
std::vector< std::vector< std::vector<int> > > table;

int* upper;
int* lower;
int* start_bit;
int* bitnum;

int imp_ss_steps;
int exp_ss_steps;
int max_ss_steps;
int total_ss_steps;
int ss_bits;
int area_size;

std::vector<int> area_pool;
std::vector<int> imp_pool;
std::vector<int> exp_pool;

void reset(){

}

#ifdef ENABLE_RES
std::set<int> res;
int res_steps;
int res_bits;
void init_res_pool(std::set<int>& res_set){
    for(int i=0;i<W;i++){
        for(int j=0;j<L;j++){
            int k = table[i][j].size()-1;
            if(k >= 0 && res_set.find(table[i][j][k]) != res_set.end()){
                pool.push_back( table[i][j][k] );
            }
        }
    }
}
#endif

void copy_map(std::map<int, dat* >& src, std::map<int, dat* >& des){
    for(auto& it: src){
        int i = it.first;
        dat* _a = new dat(it.second->_h, it.second->_w, it.second->_l);
        des.insert(std::make_pair(i, _a));
    }
}

int adjust(int curr, int max_curr, int max_n){
    return (curr * max_n ) / max_curr;
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
    int start = 0;
    int all = W*L;
    int last_x = -1;
    int last_y = -1;
#ifdef ENABLE_RES
    init_pool(res);
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
        y += (abs(cc_containers[r]->_l - areas[des]->_l) * 2);
    }
    pool.clear();
#endif
    imp_pool.clear();
    exp_pool.clear();
    area_pool.clear();
#ifdef DEBUG
    printf("IMP_SS: %d\n", imp_ss);
    printf("EXP_SS: %d\n", exp_ss.size());
#endif
    for(int i=0;i<imp_ss; i++){
        imp_pool.push_back(i);
        if(display) printf("IMP: %d\n", i);
    }
    for(auto& it : exp_ss){
        exp_pool.push_back(it);
        if(display) printf("EXP: %d\n", it);
    }
    for(auto& it : areas){
        area_pool.push_back(it.first);
        if(display) printf("AREA: %d\n", it.first);
    }
    start = 0;
    int front_bit = decimal_2_binary_size(max_ss_steps);
    int last_bit = decimal_2_binary_size(all);
    int front_num = (int)pow(2,front_bit);
    int last_num = (int)pow(2,last_bit);
    int total_ss_steps = imp_ss_steps + exp_ss_steps;
    for(int i=0; i<total_ss_steps; i++){
        int it, area_it;
        if(display){
            for(int i=start; i<start+1+front_bit+last_bit; i++){
                printf("%d",x[i]);
            }
            printf("\n");
        }
        char opd = x[start++];
        it = binary_2_decimal(front_bit, x+start);
        start += front_bit;
        area_it = binary_2_decimal(last_bit, x+start);
        start += last_bit;
        if(display) printf("It:%d, area_it: %d, all: %d, areas.size(): %d\n", it, area_it, all, area_pool.size());
        if( (opd == 0 && !imp_pool.empty()) || exp_pool.empty() ){
            //! IMPORT
            int idx_a = adjust(area_it,last_num,area_pool.size()-1);
            int a = pop_pool(area_pool, idx_a);
            int _x = areas[a]->_h;
            int _y = areas[a]->_w;
            int _z = areas[a]->_l;
            int idx_r = adjust(it, front_num,imp_pool.size()-1);
            int r = pop_pool(imp_pool, idx_r);
#ifdef DEBUG
            printf("IMP %d to ( %d, %d, %d )\n",r, areas[des]->_h , areas[des]->_w, areas[des]->_l);
#endif
            double duration = 0;
            if(last_x != -1){
                duration = ((last_x + 1) * TRAVEL_TIME);
                y += duration;
                if(display) printf("IMP TRAVEL BACK FROM %d (%lf -> %lf)\n", last_x, duration, y);
                last_x = -1;
                last_y = -1;
            }
            duration = ((areas[a]->_w +1) * TRAVEL_TIME) + (2*CONTROL_TIME);
            y += duration;
            if(display) printf("IMP TRAVEL TO %d, %d [%d] (%lf -> %lf)\n",  areas[a]->_w, areas[a]->_l, a, duration, y);
            last_x = areas[a]->_w;
            last_y = areas[a]->_l;
        }
        else{
            //! EXPORT
            int idx_r = adjust(it, front_num,exp_pool.size()-1);
            int r = pop_pool(exp_pool, idx_r);
 #ifdef DEBUG
            printf("EXP %d( %d, %d, %d ) to SS\n",r ,
                cc_containers[r]->_h, cc_containers[r]->_w, cc_containers[r]->_l);
#endif
            double duration = 0;
            if(last_x != -1){
//                duration = ((abs(cc_containers[r]->_w - last_x) + abs(cc_containers[r]->_l - last_y)) * TRAVEL_TIME);
                duration = (abs(cc_containers[r]->_w - last_x) * TRAVEL_TIME);
                y += duration;
//                if(display) printf("EXP TRAVEL FROM %d, %d TO %d, %d [%d] (%lf -> %lf)\n", last_x, last_y, cc_containers[r]->_w, cc_containers[r]->_l, r, duration, y);
                if(display) printf("EXP TRAVEL FROM %d, %d TO %d, %d [%d] (%lf -> %lf)\n", last_x, last_y, cc_containers[r]->_w, cc_containers[r]->_l, r, duration, y);
            }
            duration = ((cc_containers[r]->_w +1) * TRAVEL_TIME) + (2*CONTROL_TIME);
            y += duration;
            if(display) printf("EXP TRAVEL %d (%lf -> %lf)\n",  cc_containers[r]->_w, duration, y);
            last_x = -1;
            last_y = -1;
        }
        //Finish
        
    }
    return y;
}

int calculate_malloc_size() {
    int sbit = 0;
    int all = W*L;
#ifdef ENABLE_RES
    res_steps = res.size();
    res_bits = (decimal_2_binary_size(res_steps) + decimal_2_binary_size(all) * res_steps);
    printf("RES BITS: %d\n", res_bits);
    sbit += res_bits;
#endif
    imp_ss_steps = imp_ss;
    exp_ss_steps = exp_ss.size();
    max_ss_steps = std::max(imp_ss_steps, exp_ss_steps);
    total_ss_steps = imp_ss_steps + exp_ss_steps;
    ss_bits = (1 + decimal_2_binary_size(max_ss_steps) + decimal_2_binary_size(all)) * (total_ss_steps);
//    printf("SS BITS: (1 + %d + %d) * %d -> %d\n",decimal_2_binary_size(max_ss_steps), decimal_2_binary_size(all), total_ss_steps, ss_bits);
    sbit += ss_bits;
    return sbit;
}

void read_data(const char* file) {
    FILE *ptr = NULL;
    ptr = fopen(file, "r");
    if (ptr) {
        int n;
        int x, y, z;
        //! Read H W L
        fscanf(ptr, "%d %d %d", &H, &W, &L);
        
        table.resize(W);
        for(int i=0;i<W;i++){
            table[i].resize(L);
        }
        
        //! Read Containers
        fscanf(ptr, "%d", &n);
        for(int i=0; i<n; i++){
            fscanf(ptr, "%d %d %d", &x, &y, &z);
            dat* _a = new dat(x, y, z);
            cc_containers.insert(std::make_pair(i, _a));
        }
        //! Read Areas
        fscanf(ptr, "%d", &n);
        for(int i=0; i<n; i++){
            fscanf(ptr, "%d %d %d", &x, &y, &z);
            dat* _a = new dat(x, y, z);
            areas.insert(std::make_pair(i, _a));
        }
        //! Number of IMP SS
        fscanf(ptr, "%d", &imp_ss);
#ifdef ENABLE_LS
        //! Number of IMP LS
        fscanf(ptr, "%d", &imp_ls);
#endif
        //! Number of EXP SS
        fscanf(ptr, "%d", &n);
        int t;
        for(int i=0; i<n; i++){
            fscanf(ptr, "%d", &t);
            exp_ss.insert(t);
        }
#ifdef ENABLE_LS
        //! Number of EXP LS
        fscanf(ptr, "%d", &n);
        for(int i=0; i<n; i++){
            fscanf(ptr, "%d", &t);
            exp_ls.push_back(t);
        }
#endif
    }
#ifdef DEBUG
    for(auto& it : cc_containers){
        printf("%d %d %d\n", it.second->_h, it.second->_w, it.second->_l);
    }
#endif
    fclose(ptr);
}

void analyze(){
    std::vector<std::pair<int, dat*> > pairs;
    for (auto itr = cc_containers.begin(); itr != cc_containers.end(); ++itr){
        pairs.push_back(*itr);
    }
    std::sort(pairs.begin(), pairs.end(), [=](std::pair<int, dat*>& a, std::pair<int, dat*>& b)
    {
        return a.second->_h < b.second->_h;
    }
    );
    for(auto& it : pairs){
        table[it.second->_w][it.second->_l].push_back(it.first);
    } 
#ifdef ENABLE_RES
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
#endif
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
//    char* file_name = init(argv[1]);
    char* file_name = "./data/example_data_2.txt";
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
    double best_y = fx_function_solve(malloc_size, xgbest, true);
    printf("Best Result: %lf", best_y);

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