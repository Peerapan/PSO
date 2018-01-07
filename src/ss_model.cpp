#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <string>
#include <stack>
#include <limits>
#include <iostream>
#include <algorithm>

#include "function.h"
#include "ss_model.h"

SS_Model::SS_Model() {

}

SS_Model::SS_Model(const char*& input) {
    load_data(input);
    analyze();
    calculate_malloc_size();
}

SS_Model::~SS_Model() {
    for (auto& it : cc_containers) {
        if (it.second) {
            delete it.second;
            it.second = NULL;
        }
    }
    for (auto it : areas) {
        if (it.second) {
            delete it.second;
            it.second = NULL;
        }
    }
}

void SS_Model::load_data(const char* file) {
    FILE *ptr = NULL;
    ptr = fopen(file, "r");
    if (ptr) {
        int n;
        int x, y, z;
        //! Read H W L
        fscanf(ptr, "%d %d %d", &H, &W, &L);
        table.resize(W);
        for (int i = 0; i < W; i++) {
            table[i].resize(L);
        }
        //! Read Containers
        fscanf(ptr, "%d", &n);
        for (int i = 0; i < n; i++) {
            fscanf(ptr, "%d %d %d", &x, &y, &z);
            dat* _a = new dat(x, y, z);
            cc_containers.insert(std::make_pair(i, _a));
        }
        //! Read Areas
        fscanf(ptr, "%d", &n);
        for (int i = 0; i < n; i++) {
            fscanf(ptr, "%d %d %d", &x, &y, &z);
            dat* _a = new dat(x, y, z);
            areas.insert(std::make_pair(i, _a));
        }
        int t;
        //! Read Res
        fscanf(ptr, "%d", &n);
        for (int i = 0; i < n; i++) {
            fscanf(ptr, "%d", &t);
            res.insert(t);
        }
        //! Number of IMP SS
        fscanf(ptr, "%d", &imp_ss);
        //! Number of EXP SS
        fscanf(ptr, "%d", &n);
        for (int i = 0; i < n; i++) {
            fscanf(ptr, "%d", &t);
            exp_ss.insert(t);
        }
    }
    fclose(ptr);
}

void SS_Model::analyze() {
    std::vector<std::pair<int, dat*> > pairs;
    for (auto itr = cc_containers.begin(); itr != cc_containers.end(); ++itr) {
        pairs.push_back(*itr);
    }
    std::sort(pairs.begin(), pairs.end(), [ = ](std::pair<int, dat*>& a, std::pair<int, dat*>& b){
        return a.second->_h < b.second->_h;
    });
    for (auto& it : pairs) {
        table[it.second->_w][it.second->_l].push_back(it.first);
    }
    for (int i = 0; i < imp_ss; i++) {
        imp_pool.push_back(i);
    }
    for (auto& it : exp_ss) {
        exp_pool.push_back(it);
    }
    for (auto& it : areas) {
        area_pool.push_back(it.first);
    }
    for (int i = 0; i < W; i++) {
        for (int j = 0; j < L; j++) {
            int k = table[i][j].size() - 1;
            if (k >= 0 && res.find(table[i][j][k]) != res.end()) {
                res_pool.push_back(table[i][j][k]);
            }
        }
    }
}

int SS_Model::calculate_malloc_size() {
    int sbit = 0;
    int all = W*L;

    res_steps = res.size();
    res_bits = (decimal_2_binary_size(res_steps) + decimal_2_binary_size(all)) * res_steps;

    sbit += res_bits;
    imp_ss_steps = imp_ss;
    exp_ss_steps = exp_ss.size();
    max_ss_steps = std::max(imp_ss_steps, exp_ss_steps);
    total_ss_steps = imp_ss_steps + exp_ss_steps;
    ss_bits = (1 + decimal_2_binary_size(max_ss_steps) + decimal_2_binary_size(all)) * (total_ss_steps);
    sbit += ss_bits;

    allocate_size = sbit;
    return allocate_size;
}

SS_Model* SS_Model::clone() {
    SS_Model *m = new SS_Model();

    m->W = this->W;
    m->H = this->H;
    m->L = this->L;

    m->table.resize(W);
    for (int i = 0; i < W; i++) {
        m->table[i].resize(L);
    }

    for (auto& it : this->cc_containers) {
        int i = it.first;
        dat* _a = new dat(it.second->_h, it.second->_w, it.second->_l);
        m->cc_containers.insert(std::make_pair(i, _a));
    }
    for (auto& it : this->areas) {
        int i = it.first;
        dat* _a = new dat(it.second->_h, it.second->_w, it.second->_l);
        m->areas.insert(std::make_pair(i, _a));
    }
    for (auto& it : this->res) {
        m->res.insert(it);
    }
    m->imp_ss = this->imp_ss;
    for (auto& it : this->exp_ss) {
        m->exp_ss.insert(it);
    }

    m->analyze();
    m->calculate_malloc_size();

    return m;
}

int SS_Model::pop_area_pool(int idx) {
    int a = area_pool[idx];
    if (areas[a]->_h + 1 < H) {
        int n = areas.size();
        areas[n] = new dat(areas[a]->_h + 1, areas[a]->_w, areas[a]->_l);
        area_pool[idx] = n;
    } else {
        area_pool[idx] = area_pool[area_pool.size() - 1];
        area_pool.pop_back();
    }
    return a;
}

int SS_Model::pop_res_pool(int idx) {
    int r = res_pool[idx];
    int nt = table[cc_containers[r]->_w][cc_containers[r]->_l][cc_containers[r]->_h - 1];
    if (cc_containers[r]->_h - 1 >= 0 && res.find(nt) != res.end()) {
        int n = cc_containers.size();
        res_pool[idx] = nt;
    } else {
        res_pool[idx] = res_pool[res_pool.size() - 1];
        res_pool.pop_back();
    }
    return r;
}

int SS_Model::pop_pool(std::vector<int>& pool, int idx) {
    int r = pool[idx];
    pool[idx] = pool[pool.size() - 1];
    pool.pop_back();
    return r;
}

void SS_Model::find_res() {
    for (int i = 0; i < W; i++) {
        for (int j = 0; j < L; j++) {
            for (int k = 0; k < table[i][j].size(); k++) {
                if (exp_ss.find(table[i][j][k]) != exp_ss.end()) {
                    for (int l = k + 1; l < table[i][j].size(); l++) {
                        res.insert(table[i][j][l]);
                    }
                    break;
                }
            }
        }
    }
}

double SS_Model::fx_function_solve(int x_size, char* x, bool display) {
    double y = 0;
    int start = 0;
    int all = W*L;
    int last_x = -1;
    int last_y = -1;

    int res_bit = decimal_2_binary_size(res_steps);
    int all_bit = decimal_2_binary_size(all);
    int front_num = (int) pow(2, res_bit);
    int last_num = (int) pow(2, all_bit);
    for (int i = 0; i < res_steps; i++) {
//        if (display) {
//            printf("Binary: ");
//            for (int j = start; j < start + res_bit + all_bit; j++) {
//                printf("%d", x[j]);
//            }
//            printf("\n");
//        }
        int res_it = binary_2_decimal(res_bit, x + start);
        start += res_bit;
        int area_it = binary_2_decimal(all_bit, x + start);
        start += all_bit;
        int idx_r = adjust(res_it, front_num - 1, res_pool.size() - 1);
        int r = pop_res_pool(idx_r);
        int des = adjust(area_it, last_num - 1, area_pool.size() - 1);
        int a = pop_area_pool(des);
        int _x = cc_containers[r]->_w;
        int _y = cc_containers[r]->_l;
        double duration = 0;
        if (last_x != _x) {
            duration = (abs(last_x - _x) * TRAVEL_TIME);
            y += duration;
            if (display) printf("MOVE FROM %d, %d TO %d, %d (%lf -> %lf)\n", last_x, last_y, _x, _y, duration, y);
            last_x = _x;
            last_y = _y;
        }
        duration = CONTROL_TIME;
        y += duration;
        if (display) printf("PICK %d (%lf -> %lf)\n", r+1, duration, y);
        if (display) {
            printf("Move %d( %d, %d, %d ) to ( %d, %d, %d ) (%lf->%f)\n", r+1,
                    cc_containers[r]->_h, cc_containers[r]->_w, cc_containers[r]->_l,
                    areas[a]->_h, areas[a]->_w, areas[a]->_l,
                    duration, y);
        }
        duration = CONTROL_TIME;
        y += duration;
        if (display) printf("DROP %d (%lf -> %lf)\n", r+1, duration, y);
        last_x = areas[a]->_w;
        last_y = areas[a]->_l;
    }
#ifdef DEBUG
    printf("IMP_SS: %d\n", imp_ss);
    printf("EXP_SS: %d\n", exp_ss.size());
#endif
    int front_bit = decimal_2_binary_size(max_ss_steps);
    int last_bit = decimal_2_binary_size(all);
    front_num = (int) pow(2, front_bit);
    last_num = (int) pow(2, last_bit);
    int total_ss_steps = imp_ss_steps + exp_ss_steps;
    for (int i = 0; i < total_ss_steps; i++) {
        int it, area_it;
//        if (display) {
//            printf("Binary: ");
//            for (int j = start; j < start + 1 + front_bit + last_bit; j++) {
//                printf("%d", x[j]);
//            }
//            printf("\n");
//        }
        char opd = x[start++];
        it = binary_2_decimal(front_bit, x + start);
        start += front_bit;
        area_it = binary_2_decimal(last_bit, x + start);
        start += last_bit;
        if ((opd == 0 && !imp_pool.empty()) || exp_pool.empty()) {
            //! IMPORT
            int idx_a = adjust(area_it, last_num - 1, area_pool.size() - 1);
            int a = pop_area_pool(idx_a);
            int idx_r = adjust(it, front_num - 1, imp_pool.size() - 1);
            int r = pop_pool(imp_pool, idx_r);
#ifdef DEBUG
            printf("IMP %d to ( %d, %d, %d )\n", r, areas[des]->_h, areas[des]->_w, areas[des]->_l);
#endif
            double duration = 0;
            if (last_x != -1) {
                duration = ((last_x + 1) * TRAVEL_TIME);
                y += duration;
                if (display) printf("IMP MOVE FROM %d, %d TO SS (%lf -> %lf)\n", last_x, last_y, duration, y);
                last_x = -1;
                last_y = -1;
            }
            duration = CONTROL_TIME;
            y += duration;
            if (display) printf("PICK IMP-%d (%lf -> %lf)\n", r+1, duration, y);
            duration = ((areas[a]->_w + 1) * TRAVEL_TIME);
            y += duration;
            if (display) printf("IMP MOVE IMP-%d TO %d (%d, %d, %d) (%lf -> %lf)\n",
                    r+1, a+1, areas[a]->_h, areas[a]->_w, areas[a]->_l, duration, y);
            duration = CONTROL_TIME;
            y += duration;
            if (display) printf("DROP IMP-%d (%lf -> %lf)\n", r+1, duration, y);
            last_x = areas[a]->_w;
            last_y = areas[a]->_l;
        } else {
            //! EXPORT
            int idx_r = adjust(it, front_num - 1, exp_pool.size() - 1);
            int r = pop_pool(exp_pool, idx_r);
#ifdef DEBUG
            printf("EXP %d( %d, %d, %d ) to SS\n", r,
                    cc_containers[r]->_h, cc_containers[r]->_w, cc_containers[r]->_l);
#endif
            double duration = 0;
            duration = (abs(cc_containers[r]->_w - last_x) * TRAVEL_TIME);
            y += duration;
            if (display) printf("EXP MOVE FROM %d, %d TO %d (%d, %d, %d) (%lf -> %lf)\n",
                    last_x, last_y, r+1, cc_containers[r]->_h, cc_containers[r]->_w, cc_containers[r]->_l, duration, y);
            duration = CONTROL_TIME;
            y += duration;
            if (display) printf("PICK %d (%lf -> %lf)\n", r+1, duration, y);
            duration = ((cc_containers[r]->_w + 1) * TRAVEL_TIME);
            y += duration;
            if (display) printf("EXP MOVE %d (%d, %d, %d) TO SS (%lf -> %lf)\n", 
                    r+1, cc_containers[r]->_h, cc_containers[r]->_w, cc_containers[r]->_l, duration, y);
            duration = CONTROL_TIME;
            y += duration;
            if (display) printf("DROP %d (%lf -> %lf)\n", r+1, duration, y);
            last_x = -1;
            last_y = -1;
        }
    }
    if (last_x != -1 && last_y != -1) {
        double duration = (last_x + 1) * TRAVEL_TIME;
        y += duration;
        if (display) printf("TRAVEL BACK %d, %d (%lf -> %lf)\n", last_x, last_y, duration, y);
    }
    return y;
}

void SS_Model::display() {
    for (auto& it : cc_containers) {
        printf("%d %d %d\n", it.second->_h, it.second->_w, it.second->_l);
    }
}