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
#include "all_model.h"

#include "linear_graph.h"

All_Model::All_Model() {
}

All_Model::All_Model(const char*& input) {
    load_data(input);
    analyze();
    calculate_malloc_size();
}

All_Model::~All_Model() {
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
    for (auto it : ss_graph) {
        if (it) {
            delete it;
            it = NULL;
        }
    }
    ss_graph.clear();
    for (auto it : ls_graph) {
        if (it) {
            delete it;
            it = NULL;
        }
    }
    ls_graph.clear();
}

void All_Model::load_data(const char* file) {
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
            last_cc_container = i;
        }
        //! Read Areas
        fscanf(ptr, "%d", &n);
        for (int i = 0; i < n; i++) {
            fscanf(ptr, "%d %d %d", &x, &y, &z);
            dat* _a = new dat(x, y, z);
            areas.insert(std::make_pair(i, _a));
        }
        int t;
        //! Read Res SS
        fscanf(ptr, "%d", &n);
        for (int i = 0; i < n; i++) {
            fscanf(ptr, "%d", &t);
            res_ss.insert(t);
        }
        //! Number of IMP SS
        fscanf(ptr, "%d", &imp_ss);
        //! Number of EXP SS
        fscanf(ptr, "%d", &n);
        for (int i = 0; i < n; i++) {
            fscanf(ptr, "%d", &t);
            exp_ss.insert(t);
        }
        //! Read Res LS
        fscanf(ptr, "%d", &n);
        for (int i = 0; i < n; i++) {
            fscanf(ptr, "%d", &t);
            res_ls.insert(t);
        }
        //! Number of IMP LS
        fscanf(ptr, "%d", &imp_ls);
        //! Number of EXP LS
        fscanf(ptr, "%d", &n);
        for (int i = 0; i < n; i++) {
            fscanf(ptr, "%d", &t);
            exp_ls.insert(t);
        }
    }
    fclose(ptr);
}

void All_Model::analyze() {
    std::vector<std::pair<int, dat*> > pairs;
    for (auto itr = cc_containers.begin(); itr != cc_containers.end(); ++itr) {
        pairs.push_back(*itr);
    }
    std::sort(pairs.begin(), pairs.end(), [ = ](std::pair<int, dat*>& a, std::pair<int, dat*>& b){
        return a.second->_h < b.second->_h;
    });
    for (auto& it : pairs) {
        if (it.second->_w >= 0 && it.second->_l >= 0) {
            table[it.second->_w][it.second->_l].push_back(it.first);
        }
    }
    for (int i = 0; i < imp_ss; i++) {
        imp_ss_pool.push_back(i);
    }
    for (int i = 0; i < imp_ls; i++) {
        imp_ls_pool.push_back(i);
    }
    for (auto& it : exp_ss) {
        exp_ss_pool.push_back(it);
    }
    for (auto& it : exp_ls) {
        exp_ls_pool.push_back(it);
    }
    for (auto& it : areas) {
        area_pool.push_back(it.first);
    }
    for (int i = 0; i < W; i++) {
        for (int j = 0; j < L; j++) {
            int k = table[i][j].size() - 1;
            if (k >= 0 && res_ss.find(table[i][j][k]) != res_ss.end()) {
                res_ss_pool.push_back(table[i][j][k]);
            }
        }
    }
    for (int i = 0; i < W; i++) {
        for (int j = 0; j < L; j++) {
            int k = table[i][j].size() - 1;
            if (k >= 0 && res_ls.find(table[i][j][k]) != res_ls.end()) {
                res_ls_pool.push_back(table[i][j][k]);
            }
        }
    }
}

void All_Model::ls_analyze() {
    std::vector<int>::iterator it = area_pool.begin();
    while (it != area_pool.end()) {
        if (mark[areas[*it]->_w][areas[*it]->_l]) {
            area_pool.erase(it);
        } else {
            ++it;
        }
    }
}

int All_Model::calculate_malloc_size() {
    int sbit = 0;
    int all = W*L;

    res_ss_steps = res_ss.size();
    res_ss_bits = (decimal_2_binary_size(res_ss_steps) + decimal_2_binary_size(all)) * res_ss_steps;
    res_ls_steps = res_ls.size();
    res_ls_bits = (decimal_2_binary_size(res_ls_steps) + decimal_2_binary_size(all)) * res_ls_steps;

    sbit = 0;
    sbit += res_ss_bits;
    imp_ss_steps = imp_ss;
    exp_ss_steps = exp_ss.size();
    max_ss_steps = std::max(imp_ss_steps, exp_ss_steps);
    total_ss_steps = imp_ss_steps + exp_ss_steps;
    ss_bits = (1 + decimal_2_binary_size(max_ss_steps) + decimal_2_binary_size(all)) * (total_ss_steps);
    sbit += ss_bits;
    ss_allocate_size = sbit;

    sbit = 0;
    sbit += res_ls_bits;
    imp_ls_steps = imp_ls;
    exp_ls_steps = exp_ls.size();
    max_ls_steps = std::max(imp_ls_steps, exp_ls_steps);
    total_ls_steps = imp_ls_steps + exp_ls_steps;
    ss_bits = (1 + decimal_2_binary_size(max_ls_steps) + decimal_2_binary_size(all)) * (total_ls_steps);
    sbit += ss_bits;
    ls_allocate_size = sbit;

    if( ss_allocate_size > ls_allocate_size ){
        allocate_size = ss_allocate_size;
    }else{
        allocate_size = ls_allocate_size;
    }
    return allocate_size;
}

All_Model* All_Model::clone() {
    All_Model *m = new All_Model();

    m->W = this->W;
    m->H = this->H;
    m->L = this->L;

    m->table.resize(W);
    for (int i = 0; i < W; i++) {
        m->table[i].resize(L);
    }

    m->last_cc_container = last_cc_container;
    
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
    for (auto& it : this->res_ss) {
        m->res_ss.insert(it);
    }
    for (auto& it : this->res_ls) {
        m->res_ls.insert(it);
    }
    m->imp_ss = this->imp_ss;
    for (auto& it : this->exp_ss) {
        m->exp_ss.insert(it);
    }
    m->imp_ls = this->imp_ls;
    for (auto& it : this->exp_ls) {
        m->exp_ls.insert(it);
    }

    m->analyze();
    m->calculate_malloc_size();

    for(auto& it : this->ss_graph){
        m->ss_graph.push_back( it->clone() );
    }
    
    for(auto& it : this->ls_graph){
        m->ls_graph.push_back( it->clone() );
    }
    
    return m;
}

int All_Model::pop_area_pool(int idx) {
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

int All_Model::pop_res_ss_pool(int idx) {
    int r = res_ss_pool[idx];
    int nt = table[cc_containers[r]->_w][cc_containers[r]->_l][cc_containers[r]->_h - 1];
    if (cc_containers[r]->_h - 1 >= 0 && res_ss.find(nt) != res_ss.end()) {
        res_ss_pool[idx] = nt;
    } else {
        res_ss_pool[idx] = res_ss_pool[res_ss_pool.size() - 1];
        res_ss_pool.pop_back();
    }
    return r;
}

int All_Model::pop_res_ls_pool(int idx) {
    int r = res_ls_pool[idx];
    int nt = table[cc_containers[r]->_w][cc_containers[r]->_l][cc_containers[r]->_h - 1];
    if (cc_containers[r]->_h - 1 >= 0 && res_ls.find(nt) != res_ls.end()) {
        res_ls_pool[idx] = nt;
    } else {
        res_ls_pool[idx] = res_ls_pool[res_ls_pool.size() - 1];
        res_ls_pool.pop_back();
    }
    return r;
}

int All_Model::pop_pool(std::vector<int>& pool, int idx) {
    int r = pool[idx];
    pool[idx] = pool[pool.size() - 1];
    pool.pop_back();
    return r;
}

void All_Model::find_res() {
    for (int i = 0; i < W; i++) {
        for (int j = 0; j < L; j++) {
            for (int k = 0; k < table[i][j].size(); k++) {
                if (exp_ss.find(table[i][j][k]) != exp_ss.end()) {
                    for (int l = k + 1; l < table[i][j].size(); l++) {
                        res_ss.insert(table[i][j][l]);
                    }
                    break;
                }
            }
        }
    }
}

double All_Model::fx_function_solve(int x_size, char* x, bool edited) {
    double y = 0;
    int start = 0;
    int all = W*L;
    int last_x = -1;
    int last_y = -1;

    if (edited) {
        mark.resize(W);
        for (int i = 0; i < W; i++) {
            mark[i].resize(L);
            for (int j = 0; j < L; j++) {
                mark[i][j] = false;
            }
        }
    }

    int res_ss_bit = decimal_2_binary_size(res_ss_steps);
    int all_bit = decimal_2_binary_size(all);
    int front_num = (int) pow(2, res_ss_bit);
    int last_num = (int) pow(2, all_bit);
    for (int i = 0; i < res_ss_steps; i++) {
        int res_it = binary_2_decimal(res_ss_bit, x + start);
        start += res_ss_bit;
        int area_it = binary_2_decimal(all_bit, x + start);
        start += all_bit;
        int idx_r = adjust(res_it, front_num - 1, res_ss_pool.size() - 1);
        int r = pop_res_ss_pool(idx_r);
        int des = adjust(area_it, last_num - 1, area_pool.size() - 1);
        int a = pop_area_pool(des);
        int _x = cc_containers[r]->_w;
        int _y = cc_containers[r]->_l;
        double duration = 0;
        if (last_x != _x || last_y != _y) {
            duration = (abs(last_x - _x) * TRAVEL_TIME);
            if (edited) {
                ss_graph.push_back(new SlopeTimeGraph((int) y, (int) y + duration, last_x, _x));
                printf("MOVE FROM %d, %d TO %d, %d (%lf + %lf -> %lf)\n", last_x, last_y, _x, _y, y, duration, y + duration);
            }
            y += duration;
            last_x = _x;
            last_y = _y;
        }
        duration = CONTROL_TIME;
        if (edited) {
            ss_graph.push_back(new StableTimeGraph((int) y, (int) y + duration, _x));
            printf("PICK %d (%lf + %lf -> %lf)\n", r + 1, y, duration, y + duration);
        }
        y += duration;
        duration = (abs(areas[a]->_w - cc_containers[r]->_w) * TRAVEL_TIME);
         if (edited){
            ss_graph.push_back(new SlopeTimeGraph((int) y, (int) y + duration, _x, areas[a]->_w));
            printf("Move %d( %d, %d, %d ) to ( %d, %d, %d ) (%lf + %lf -> %f)\n", r + 1,
                cc_containers[r]->_h, cc_containers[r]->_w, cc_containers[r]->_l,
                areas[a]->_h, areas[a]->_w, areas[a]->_l,
                y, duration, y + duration);
        }
        if(edited){
            mark[cc_containers[r]->_w][cc_containers[r]->_l] = true;
            mark[areas[a]->_w][areas[a]->_l] = true;
        }
        cc_containers[r]->_h = areas[a]->_h;
        cc_containers[r]->_w = areas[a]->_w;
        cc_containers[r]->_l = areas[a]->_l;
        y += duration;
        duration = CONTROL_TIME;
        if (edited) {
            ss_graph.push_back(new StableTimeGraph((int) y, (int) y + duration, areas[a]->_w));
            printf("DROP %d (%lf + %lf -> %lf)\n", r + 1, y, duration, y + duration);
        }
        y += duration;
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
        char opd = x[start++];
        it = binary_2_decimal(front_bit, x + start);
        start += front_bit;
        area_it = binary_2_decimal(last_bit, x + start);
        start += last_bit;
        if ((opd == 0 && !imp_ss_pool.empty()) || exp_ss_pool.empty()) {
            //! IMPORT
            int idx_a = adjust(area_it, last_num - 1, area_pool.size() - 1);
            int a = pop_area_pool(idx_a);
            int idx_r = adjust(it, front_num - 1, imp_ss_pool.size() - 1);
            int r = pop_pool(imp_ss_pool, idx_r);
            double duration = 0;
            if (last_x != -1) {
                duration = ((last_x + 1) * TRAVEL_TIME);
                if (edited) {
                    ss_graph.push_back(new SlopeTimeGraph((int) y, (int) y + duration, last_x, -1));
                    printf("IMP MOVE FROM %d, %d TO SS (%lf + %lf -> %lf)\n", last_x, last_y, y, duration, y + duration);
                }
                y += duration;
            }
            duration = CONTROL_TIME;
            if (edited) {
                ss_graph.push_back(new StableTimeGraph((int) y, (int) y + duration, -1));
                printf("PICK IMP-%d (%lf + %lf -> %lf)\n", r + 1, y, duration, y + duration);
            }
            y += duration;
            duration = ((areas[a]->_w + 1) * TRAVEL_TIME);
            if (edited) {
                ss_graph.push_back(new SlopeTimeGraph((int) y, (int) y + duration, -1, areas[a]->_w));
                printf("IMP MOVE IMP-%d TO %d (%d, %d, %d) (%lf + %lf -> %lf)\n",
                        r + 1, a + 1, areas[a]->_h, areas[a]->_w, areas[a]->_l,
                        y, duration, y + duration);
                mark[areas[a]->_w][areas[a]->_l] = true;
                dat* _a = new dat(areas[a]->_h, areas[a]->_w, areas[a]->_l);
                cc_containers.insert(std::make_pair(++last_cc_container, _a));
            }
            y += duration;
            duration = CONTROL_TIME;
            if (edited) {
                ss_graph.push_back(new StableTimeGraph((int) y, (int) y + duration, areas[a]->_w));
                printf("DROP IMP-%d (%lf + %lf -> %lf)\n", r + 1,
                        y, duration, y + duration);
            }
            y += duration;
            last_x = areas[a]->_w;
            last_y = areas[a]->_l;
        } else {
            //! EXPORT
            int idx_r = adjust(it, front_num - 1, exp_ss_pool.size() - 1);
            int r = pop_pool(exp_ss_pool, idx_r);
            double duration = 0;
            if (last_x != cc_containers[r]->_w || last_y != cc_containers[r]->_l){
                duration = (abs(cc_containers[r]->_w - last_x) * TRAVEL_TIME);
                 if (edited){
                    ss_graph.push_back(new SlopeTimeGraph((int) y, (int) y + duration, last_x, cc_containers[r]->_w));
                    printf("EXP MOVE FROM %d, %d TO %d (%d, %d, %d) (%lf + %lf -> %lf)\n",
                            last_x, last_y, r + 1, cc_containers[r]->_h, cc_containers[r]->_w, cc_containers[r]->_l, y, duration, y + duration);
                 }
                y += duration;
            }
            duration = CONTROL_TIME;
            if (edited) {
                ss_graph.push_back(new StableTimeGraph((int) y, (int) y + duration, cc_containers[r]->_w));
                printf("PICK %d (%lf + %lf -> %lf)\n", r + 1, y, duration, y + duration);
            }
            y += duration;
            duration = ((cc_containers[r]->_w + 1) * TRAVEL_TIME);
            if (edited) {
                ss_graph.push_back(new SlopeTimeGraph((int) y, (int) y + duration, cc_containers[r]->_w, -1));
                printf("EXP MOVE %d (%d, %d, %d) TO SS (%lf + %lf -> %lf)\n",
                        r + 1, cc_containers[r]->_h, cc_containers[r]->_w, cc_containers[r]->_l, y, duration, y + duration);
                mark[cc_containers[r]->_w][cc_containers[r]->_l] = true;
                cc_containers[r]->_h = 0;
                cc_containers[r]->_w = -1;
                cc_containers[r]->_l = -1;
            }
            y += duration;
            duration = CONTROL_TIME;
            if (edited) {
                ss_graph.push_back(new StableTimeGraph((int) y, (int) y + duration, -1));
                printf("DROP %d (%lf + %lf -> %lf)\n", r + 1, y, duration, y + duration);
            }
            y += duration;
            last_x = -1;
            last_y = -1;
        }
    }
    if (last_x != -1 || last_y != -1) {
        double duration = (last_x + 1) * TRAVEL_TIME;
        if (edited) {
            ss_graph.push_back(new SlopeTimeGraph((int) y, (int) y + duration, last_x, -1));
            printf("TRAVEL BACK %d, %d (%lf + %lf -> %lf)\n", last_x, last_y, y, duration, y + duration);
        }
        y += duration;
    }
    return y;
}

double All_Model::fx_function_solve_2(int x_size, char* x, bool edited) {
    int time_counter = 0;

    double y = 0;
    int start = 0;
    int all = W*L;

    int res_ls_bit = decimal_2_binary_size(res_ls_steps);
    int all_bit = decimal_2_binary_size(all);
    int front_num = (int) pow(2, res_ls_bit);
    int last_num = (int) pow(2, all_bit);
    for (int i = 0; i < res_ls_steps; i++) {
        std::vector<TimeGraph*> temp_graph;
        int res_it = binary_2_decimal(res_ls_bit, x + start);
        start += res_ls_bit;
        int area_it = binary_2_decimal(all_bit, x + start);
        start += all_bit;
        int idx_r = adjust(res_it, front_num - 1, res_ls_pool.size() - 1);
        int r = pop_res_ls_pool(idx_r);
        int des = adjust(area_it, last_num - 1, area_pool.size() - 1);
        int a = pop_area_pool(des);
        int _x = cc_containers[r]->_w;
        int _y = cc_containers[r]->_l;

        double t_y = y;
        double t_duration_0 = 0;
        double t_duration_1 = 0;
        double t_duration_2 = 0;
        double t_duration_3 = 0;
        double t_duration_4 = 0;
        double shift = 0, total_shift = 0;

        t_duration_0 = (abs(W - _x) * TRAVEL_TIME);
        shift = (double) check_ss_slope(time_counter, (int) t_y, (int) t_duration_0, W, _x);
        total_shift += shift;
        t_y += shift;
        t_y += t_duration_0;

        t_duration_1 = CONTROL_TIME;
        shift = (double) check_ss_stable(time_counter, (int) t_y, (int) t_duration_1, _x);
        total_shift += shift;
        t_y += shift;
        t_y += t_duration_1;

        t_duration_2 = (abs(areas[a]->_w - _x) * TRAVEL_TIME);
        if (t_duration_2 > 0) {
            shift = (double) check_ss_slope(time_counter, (int) t_y, (int) t_duration_2, _x, areas[a]->_w);
            total_shift += shift;
            t_y += shift;
        }
        t_y += t_duration_2;

        t_duration_3 = CONTROL_TIME;
        shift = (double) check_ss_stable(time_counter, (int) t_y, (int) t_duration_3, areas[a]->_w);
        total_shift += shift;
        t_y += shift;
        t_y += t_duration_3;

        t_duration_4 = (abs(areas[a]->_w - W) * TRAVEL_TIME);
        shift = (double) check_ss_slope(time_counter, (int) t_y, (int) t_duration_2, areas[a]->_w, W);
        total_shift += shift;
        t_y += shift;
        t_y += t_duration_4;
        if (edited) {
            if (total_shift > 0) {
                printf("WAIT %lf (%f + %f -> %f)\n", total_shift, y, total_shift, y + total_shift);
            };
            y += total_shift;
            printf("MOVE FROM LS TO %d, %d (%lf + %lf -> %lf)\n", _x, _y,
                    y, t_duration_0, y + t_duration_0);
            y += t_duration_0;
            printf("PICK %d (%lf + %lf -> %lf)\n", r + 1, y, t_duration_1, y + t_duration_1);
            y += t_duration_1;
            printf("Move %d( %d, %d, %d ) to ( %d, %d, %d ) (%lf +%lf -> %f)\n", r + 1,
                    cc_containers[r]->_h, cc_containers[r]->_w, cc_containers[r]->_l,
                    areas[a]->_h, areas[a]->_w, areas[a]->_l,
                    y, t_duration_2, y + t_duration_2);
            y += t_duration_2;
            printf("DROP %d (%lf + %lf -> %lf)\n", r + 1,
                    y, t_duration_3, y + t_duration_3);
            y += t_duration_3;
            printf("Move ( %d, %d, %d ) to LS (%lf + %lf -> %lf)\n",
                    areas[a]->_h, areas[a]->_w, areas[a]->_l,
                    y, t_duration_4, y + t_duration_4);
            y += t_duration_4;
        } else {
            y += total_shift;
            y += t_duration_0;
            y += t_duration_1;
            y += t_duration_2;
            y += t_duration_3;
            y += t_duration_4;
        }
    }
#ifdef DEBUG
    printf("IMP_LS: %d\n", imp_ls);
    printf("EXP_LS: %d\n", exp_ls.size());
#endif
    int front_bit = decimal_2_binary_size(max_ls_steps);
    int last_bit = decimal_2_binary_size(all);
    front_num = (int) pow(2, front_bit);
    last_num = (int) pow(2, last_bit);
    int total_ls_steps = imp_ls_steps + exp_ls_steps;
    for (int i = 0; i < total_ls_steps; i++) {
        int it, area_it;
        char opd = x[start++];
        it = binary_2_decimal(front_bit, x + start);
        start += front_bit;
        area_it = binary_2_decimal(last_bit, x + start);
        start += last_bit;
        if ((opd == 0 && !imp_ls_pool.empty()) || exp_ls_pool.empty()) {
            //! IMPORT
            int idx_a = adjust(area_it, last_num - 1, area_pool.size() - 1);
            int a = pop_area_pool(idx_a);
            int idx_r = adjust(it, front_num - 1, imp_ls_pool.size() - 1);
            int r = pop_pool(imp_ls_pool, idx_r);

            double t_y = y;
            double t_duration_0 = 0;
            double t_duration_1 = 0;
            double t_duration_2 = 0;
            double t_duration_3 = 0;
            double t_duration_4 = 0;
            double shift = 0, total_shift = 0;

            t_duration_0 = CONTROL_TIME;
            shift = (double) check_ss_stable(time_counter, (int) t_y, (int) t_duration_0, W);
            total_shift += shift;
            t_y += shift;
            t_y += t_duration_0;

            t_duration_1 = (abs(areas[a]->_w - W) * TRAVEL_TIME);
            shift = (double) check_ss_slope(time_counter, (int) t_y, (int) t_duration_1, W, areas[a]->_w);
            total_shift += shift;
            t_y += shift;
            t_y += t_duration_1;

            t_duration_2 = CONTROL_TIME;
            shift = (double) check_ss_stable(time_counter, (int) t_y, (int) t_duration_2, areas[a]->_w);
            total_shift += shift;
            t_y += shift;
            t_y += t_duration_2;

            t_duration_3 = (abs(W - areas[a]->_w) * TRAVEL_TIME);
            shift = (double) check_ss_slope(time_counter, (int) t_y, (int) t_duration_3, areas[a]->_w, W);
            total_shift += shift;
            t_y += shift;
            t_y += t_duration_3;

            if (edited) {
                if (total_shift > 0) {
                    printf("WAIT %lf (%lf + %lf -> %lf)\n", total_shift, y, total_shift, y + total_shift);
                }
                y += total_shift;
                printf("PICK IMP-%d (%lf + %lf -> %lf)\n", r + 1, y, t_duration_0, y + t_duration_0);
                y += t_duration_0;
                printf("IMP MOVE IMP-%d TO %d (%d, %d, %d) (%lf + %lf -> %lf)\n",
                        r + 1, a + 1, areas[a]->_h, areas[a]->_w, areas[a]->_l,
                        y, t_duration_1, y + t_duration_1);
                y += t_duration_1;
                printf("DROP IMP-%d (%lf + %lf -> %lf)\n", r + 1, y, t_duration_2, y + t_duration_2);
                y += t_duration_2;
                printf("Move ( %d, %d, %d ) to LS (%lf + %lf -> %lf)\n",
                        areas[a]->_h, areas[a]->_w, areas[a]->_l,
                        y, t_duration_3, y + t_duration_3);
                y += t_duration_3;
            } else {
                y += total_shift;
                y += t_duration_0;
                y += t_duration_1;
                y += t_duration_2;
                y += t_duration_3;
            }
        } else {
            //! EXPORT
            int idx_r = adjust(it, front_num - 1, exp_ls_pool.size() - 1);
            int r = pop_pool(exp_ls_pool, idx_r);

            double t_y = y;
            double t_duration_0 = 0;
            double t_duration_1 = 0;
            double t_duration_2 = 0;
            double t_duration_3 = 0;
            double t_duration_4 = 0;
            double shift = 0, total_shift = 0;

            t_duration_0 = abs(cc_containers[r]->_w - W) * TRAVEL_TIME;
            shift = (double) check_ss_slope(time_counter, (int) t_y, (int) t_duration_0, W, cc_containers[r]->_w);
            total_shift += shift;
            t_y += shift;
            t_y += t_duration_0;

            t_duration_1 = CONTROL_TIME;
            shift = (double) check_ss_stable(time_counter, (int) t_y, (int) t_duration_1, cc_containers[r]->_w);
            total_shift += shift;
            t_y += shift;
            t_y += t_duration_1;

            t_duration_2 = abs(W - cc_containers[r]->_w) * TRAVEL_TIME;
            shift = (double) check_ss_slope(time_counter, (int) t_y, (int) t_duration_2, cc_containers[r]->_w, W);
            total_shift += shift;
            t_y += shift;
            t_y += t_duration_2;

            t_duration_3 = CONTROL_TIME;
            shift = (double) check_ss_stable(time_counter, (int) t_y, (int) t_duration_3, W);
            total_shift += shift;
            t_y += shift;
            t_y += t_duration_3;

            if (edited) {
                if (total_shift > 0) {
                    printf("WAIT %lf (%lf + %lf -> %lf)\n", total_shift, y, total_shift, y + total_shift);
                }
                y += total_shift;
                printf("EXP MOVE FROM LS TO %d (%d, %d, %d) (%lf + %lf -> %lf)\n",
                        r + 1, cc_containers[r]->_h, cc_containers[r]->_w, cc_containers[r]->_l,
                        y, t_duration_0, y + t_duration_0);
                y += t_duration_0;
                printf("PICK %d (%lf + %lf -> %lf)\n", r + 1,
                        y, t_duration_1, y + t_duration_1);
                y += t_duration_1;
                printf("EXP MOVE %d (%d, %d, %d) TO LS (%lf + %lf -> %lf)\n",
                        r + 1, cc_containers[r]->_h, cc_containers[r]->_w, cc_containers[r]->_l,
                        y, t_duration_2, y + t_duration_2);
                y += t_duration_2;
                printf("DROP %d (%lf + %lf -> %lf)\n", r + 1,
                        y, t_duration_3, y + t_duration_3);
                y += t_duration_3;
            } else {
                y += total_shift;
                y += t_duration_0;
                y += t_duration_1;
                y += t_duration_2;
                y += t_duration_3;
            }
        }
    }
    return y;
}

void All_Model::display() {
    printf("===============================================\n");
    printf("----- Containers -----\n");
    for (auto& it : cc_containers) {
        printf("%d: %d %d %d\n", it.first, it.second->_h, it.second->_w, it.second->_l);
    }
    if (mark.size() > 0) {
        printf("----- MARK -----\n");
        for (int i = 0; i < W; i++) {
            for (int j = 0; j < L; j++) {
                if (mark[i][j]) {
                    printf("%d, %d\n", i, j);
                }
            }
        }
    }
    printf("----- AREA POOL -----\n");
    for (auto& it : area_pool) {
        printf("%d, %d, %d\n", areas[it]->_h, areas[it]->_w, areas[it]->_l);
    }
    printf("----- GRAPH -----\n");
    for(int i=0; i<ss_graph.size(); i++){
        if(ss_graph[i]->get_type() == 1){
            static_cast<SlopeTimeGraph*>(ss_graph[i])->display();
        }else{
            static_cast<StableTimeGraph*>(ss_graph[i])->display();
        }
    }
    printf("===============================================\n");
}

int All_Model::check_ss_slope(int& tc, int tt, int d, int a, int b) {
    TimeGraph* tg = new SlopeTimeGraph(tt, tt + d, a, b);
    int shift = check_ss(tg, tc, tt);
    delete tg;
    return shift;
}

int All_Model::check_ss_stable(int& tc, int tt, int d, int a) {
    TimeGraph* tg = new StableTimeGraph(tt, tt + d, a);
    int shift = check_ss(tg, tc, tt);
    delete tg;
    return shift;
}

int All_Model::check_ss(TimeGraph* src, int& time_counter, int start_time) {
    int shifter = 0;
    int i = start_time;
    int j = start_time;
    while (time_counter < ss_graph.size()) {
        bool ret = TimeGraph::compare(ss_graph[time_counter], src, i + shifter, j);
        if (ret) {
            shifter++;
        } else {
            i++;
            j++;
        }
        if (ss_graph[time_counter]->outer(i + shifter)) time_counter++;
        if (src->outer(j)) break;
    }
    return shifter;
}
