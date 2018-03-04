#include <linear_graph.h>

#include <math.h>

int TimeGraph::get_type() {
    return 0;
}

bool TimeGraph::inner(int x) {
    return x >= min_x && x <= max_x;
}

bool TimeGraph::outer(int x) {
    return x > max_x;
}

bool TimeGraph::compare(TimeGraph* a, TimeGraph* b, int i, int j) {
    if (a->get_value(i) >= b->get_value(j)) {
        return true;
    }
    return false;
}

std::string TimeGraph::toString() {
    char _str[100];
    sprintf(_str, "Move from %d to %d at %d to %d", min_y, max_y, min_x, max_x);
    return _str;
}

void TimeGraph::set_mode(int _mode, int _counter, bool _is_wait){
    mode = _mode;
    counter = _counter;
    is_wait = _is_wait;
}

void TimeGraph::get_mode(){
    switch (mode) {
      case 0:
        printf("(RES-%d-%d)", counter, is_wait);
      break;
      case 1:
        printf("(IMP-%d-%d)", counter, is_wait);
      break;
      case 2:
        printf("(EXP-%d-%d)", counter, is_wait);
      break;
      case 3:
        printf("(BACK)");
      break;
      default:
      break;
    }
}

void TimeGraph::display() {
    for (int i = min_x; i < max_x; i++) {
        int j = get_value(i);
        printf("%d %d\n", i, j);
    }
    printf("-\n");
}

SlopeTimeGraph::SlopeTimeGraph(int _min_x, int _max_x, int _min_y, int _max_y) {
    min_x = _min_x;
    max_x = _max_x;
    min_y = _min_y;
    max_y = _max_y;

    x_diff = max_x - min_x;
    x_diff /= const_travel_time;
    y_diff = max_y - min_y;

//    printf("X:Y %d %d\n", x_diff, y_diff);
}

int SlopeTimeGraph::get_type() {
    return 1;
}

int SlopeTimeGraph::get_value(int x) {
    if( x_diff == 0 ) return min_y;
    x -= min_x;
    x /= const_travel_time;
    x *= y_diff;
    x /= x_diff;
    return x + min_y;
}

TimeGraph* SlopeTimeGraph::clone() {
    TimeGraph* n = new SlopeTimeGraph(this->min_x, this->max_x, this->min_y, this->max_y);
    return n;
}

StableTimeGraph::StableTimeGraph(int _min_x, int _max_x, int _y) {
    min_x = _min_x;
    max_x = _max_x;
    min_y = _y;
    max_y = _y;
}

int StableTimeGraph::get_type() {
    return 2;
}

int StableTimeGraph::get_value(int x) {
    return max_y;
}

TimeGraph* StableTimeGraph::clone() {
    TimeGraph* n = new StableTimeGraph(this->min_x, this->max_x, this->min_y);
    return n;
}
