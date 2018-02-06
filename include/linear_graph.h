#ifndef LINEAR_GRAPH_H
#define LINEAR_GRAPH_H

#include <string>

// y=mx+b

class TimeGraph{
protected:
    int min_x, max_x;
    int min_y, max_y;
public:
    TimeGraph(){
    
    }
    ~TimeGraph(){
    
    }
    //By default
     virtual int get_value(int x){
         return 0;
    }
    bool outer(int x){
        return x > max_x;
    }
    static bool compare(TimeGraph a, TimeGraph b, int i, int j){
        if(a.get_value(i) == b.get_value(j)){
                return true;
        }
        return false;
    }
    
    std::string toString(){
        char _str[100];
        sprintf(_str, "Move from %d to %d at %d to %d", min_y, max_y,min_x, max_x );
        return _str;
    }
};

class SlopeTimeGraph : public TimeGraph{
private:
    double m;
public:
    SlopeTimeGraph(){
    
    }
    SlopeTimeGraph(int _min_x, int _max_x, int _min_y, int _max_y){
        min_x = _min_x;
        max_x = _max_x;
        min_y = _min_y;
        max_y = _max_y;
    }
    int get_value(int x){
		int ret = x;
		ret *= (max_y - min_y);
		ret /= (max_x - min_x);
		return ret;
    }
};

class StableTimeGraph : public TimeGraph{
public:
    StableTimeGraph(int _min_x, int _max_x, int _y){
       min_x = _min_x;
       max_x = _max_x;
       min_y = _y;
       max_y = _y;
    }
    int get_value(int x){
        return max_y;
    }
};

#endif /* LINEAR_GRAPH_H */

