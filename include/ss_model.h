#ifndef SS_MODEL_H
#define SS_MODEL_H

#include "model.h"

class SS_Model : Model {
private:
    int W, H, L;
    int allocate_size;
    int imp_ss, imp_ls;
    int res_steps;
    int imp_ss_steps;
    int exp_ss_steps;
    int max_ss_steps;
    int total_ss_steps;
    int res_bits;
    int ss_bits;
    int area_size;

    std::set<int> res;
    std::set<int> exp_ss;
    std::set<int> exp_ls;

    std::map<int, dat*> cc_containers;
    std::map<int, dat*> areas;
    std::vector< std::vector< std::vector<int> > > table;

    std::vector<int> area_pool;
    std::vector<int> imp_pool;
    std::vector<int> exp_pool;
    std::vector<int> res_pool;

    const static int TRAVEL_TIME = 3;
    const static int CONTROL_TIME = 28;

    void load_data(const char* file);
    void analyze();
    int calculate_malloc_size();

    void find_res();

public:
    SS_Model();
    SS_Model(const char*& input);
    ~SS_Model();
    SS_Model* clone();

    int pop_pool(std::vector<int>& pool, int idx);
    int pop_area_pool(int a);
    int pop_res_pool(int a);

    double fx_function_solve(int x_size, char* x, bool display);

    inline int get_bit_size() const {
        return allocate_size;
    }

    void display();
};

#endif /* MODEL_H */
