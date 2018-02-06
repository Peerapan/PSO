#include "linear_graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits>
#include <vector>

int main(int argc, char** argv) {
    std::vector<TimeGraph> ss_arr = {
       SlopeTimeGraph(43, 44, 1, 2),
       StableTimeGraph(45, 47, 3),
       SlopeTimeGraph(48, 49, 3, 4),
       StableTimeGraph(50, 53, 5),
       SlopeTimeGraph(54, 57, 4, 1),
       StableTimeGraph(58, 60, 1),
       SlopeTimeGraph(61, 62, 2, 3),
       StableTimeGraph(63, 65, 3),
       SlopeTimeGraph(66, 68, 4, 6),
       StableTimeGraph(69,71, 6),
       SlopeTimeGraph(72, 76, 5, 1),
    };
    std::vector<TimeGraph> ls_arr = {
       SlopeTimeGraph(43, 49, 8, 2),
       StableTimeGraph(50, 52, 2),
       SlopeTimeGraph(53, 58, 3, 8)
    };
    
    int i=43;
    int c=0;
    int ls=0, ss=0;
    
    while(ls < ls_arr.size() && ss < ss_arr.size()){
        bool ret = TimeGraph::compare(ls_arr[ls], ss_arr[ss], i, i+c);
        if(ret){
            c++;
        }else{
            i++;
        }
        if(ls_arr[ls].outer(i)) ls++;
        if(ss_arr[ss].outer(i+c)) ss++;
    }
    printf("%d\n", c);
    
    return 0;
}

