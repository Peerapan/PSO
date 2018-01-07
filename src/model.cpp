#include "model.h"

Model::Model() {

}

Model::Model(const char*& input) {
    load_data(input);
}

Model::~Model() {
}

void Model::load_data(const char* file) {
    FILE *ptr = NULL;
    ptr = fopen(file, "r");
    if (ptr) {
        //! Do Nothing
    }
    fclose(ptr);
}

Model* Model::clone() {
    Model *m = new Model();
    return m;
}

double Model::fx_function_solve(int x_size, char* x, bool display) {
    double y = 0;
    return y;
}

void Model::display() {
    //! Do Nothing
}
