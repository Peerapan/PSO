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
private:
    int allocate_size;

    virtual void load_data(const char* file);
    
public:
    Model();
    Model(const char*& input);
    ~Model();
    
    virtual Model* clone();
    virtual double fx_function_solve(int x_size, char* x, bool display);

    inline int get_bit_size() const {
        return allocate_size;
    }

    virtual void display();
};

#endif /* MODEL_H */

