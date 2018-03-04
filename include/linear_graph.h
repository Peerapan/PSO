#ifndef LINEAR_GRAPH_H
#define LINEAR_GRAPH_H

#include <string>

const int const_travel_time = 2;

// y=mx+b

class TimeGraph {
protected:
    int min_x, max_x;
    int min_y, max_y;
    int mode, counter;
    bool is_wait;
public:
    TimeGraph(){

    }
    virtual ~TimeGraph() {

    }

    virtual void set_mode(int _mode, int _counter, bool is_wait = false);
    virtual void get_mode();

    //By default
    virtual int get_type();
    virtual int get_value(int x) = 0;
    virtual TimeGraph* clone() = 0;

    bool inner(int x);
    bool outer(int x);
    static bool compare(TimeGraph* a, TimeGraph* b, int i, int j);
    std::string toString();

    virtual void display();
};

class SlopeTimeGraph : public TimeGraph {
private:
    int y_diff;
    int x_diff;
public:
    SlopeTimeGraph(){

    }
    SlopeTimeGraph(int _min_x, int _max_x, int _min_y, int _max_y);

    int get_type();
    int get_value(int x);

     TimeGraph* clone();
};

class StableTimeGraph : public TimeGraph {
public:
    StableTimeGraph(){

    }
    StableTimeGraph(int _min_x, int _max_x, int _y);

    int get_type();
    int get_value(int x);

    TimeGraph* clone();
};

#endif /* LINEAR_GRAPH_H */
