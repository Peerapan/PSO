#ifndef MODEL_H
#define MODEL_H

#include <set>
#include <map>
#include <vector>

struct dat {
    int _h;
    int _w;
    int _l;

    dat(int x, int y, int z) : _h(x), _w(y), _l(z) {
    }

    bool operator<(const dat& b) const {
        if (_h != b._h) return _h < b._h;
        else if (_w != b._w) return _w < b._w;
        else return _l < b._l;
    }
};

class Model {
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

    const static int TRAVEL_TIME = 2;
    const static int CONTROL_TIME = 10;

    void load_data(const char* file);
    void analyze();
    int calculate_malloc_size();

public:
    Model();
    Model(const char*& input);
    ~Model();
    Model* clone();
    double fx_function_solve(int x_size, char* x, bool display);

    inline int get_bit_size() const {
        return allocate_size;
    }

    void display();
};

#endif /* MODEL_H */

