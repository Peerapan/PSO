#ifndef All_MODEL_H
#define All_MODEL_H

#include <vector>
#include "model.h"

class TimeGraph;

class All_Model : Model {
private:
    int W, H, L;

    int allocate_size = 0;
    int ss_allocate_size = 0;
    int ls_allocate_size = 0;

    int imp_ss, imp_ls;
    int res_ss_steps;
    int res_ls_steps;
    int imp_ss_steps;
    int exp_ss_steps;
    int max_ss_steps;
    int total_ss_steps;
    int imp_ls_steps;
    int exp_ls_steps;
    int max_ls_steps;
    int total_ls_steps;
    int res_ss_bits;
    int res_ls_bits;
    int ss_bits;
    int ls_bits;
    int area_size;

    std::set<int> res_ss;
    std::set<int> res_ls;
    std::set<int> exp_ss;
    std::set<int> exp_ls;

    int last_cc_container;
    std::map<int, dat*> cc_containers;
    std::map<int, dat*> areas;
    std::vector< std::vector< std::vector<int> > > table;
    std::vector< std::vector< bool > > mark;

    std::vector<int> area_pool;
    std::vector<int> imp_ls_pool;
    std::vector<int> imp_ss_pool;
    std::vector<int> exp_ls_pool;
    std::vector<int> exp_ss_pool;
    std::vector<int> res_ss_pool;
    std::vector<int> res_ls_pool;

    std::vector<TimeGraph*> ss_graph;
    std::vector<TimeGraph*> ls_graph;

    const static int TRAVEL_TIME = 2;
    const static int CONTROL_TIME = 10;

    void load_data(const char* file);
    void analyze();
    int calculate_malloc_size();

    void find_res();
    int check_ss(TimeGraph* src, int& time_counter, int start_time);

public:
    All_Model();
    All_Model(const char*& input);

    ~All_Model();
    All_Model* clone();

    int pop_pool(std::vector<int>& pool, int idx);
    int pop_area_pool(int a);
    int pop_res_ss_pool(int a);
    int pop_res_ls_pool(int a);

    int check_ss_slope(int& tc, int tt, int d, int a, int b);
    int check_ss_stable(int& tc, int tt, int d, int _x);
    int check_ss(std::vector<TimeGraph*>& src, int& _time_counter, int start_time);

    void ls_analyze();
    double fx_function_solve(int x_size, char* x, bool edited = false);
    double fx_function_solve_2(int x_size, char* x, bool edited = false);

    inline int get_bit_size() const {
        return allocate_size;
    }

    void display();
};

#endif /* MODEL_H */
