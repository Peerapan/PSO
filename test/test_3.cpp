#include "function.h"
#include "all_model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits>
#include <time.h>

//#define DEBUG

#define WEIGHT 1000

int main(int argc, const char** argv) {
    const char* file_name = argv[1];

    std::map<std::string, double> configs;
    read_configs(configs);

    All_Model* master = new All_Model(file_name);
    int malloc_size = master->get_bit_size();

    double Pbest1 = std::numeric_limits<double>::max();
    double Gbest1 = std::numeric_limits<double>::max();
    int popsize = (int)configs["POPSIZE"];
    char** x = (char**) malloc(sizeof (char*)*popsize);
    char** xpbest = (char **) malloc(sizeof (char*)*popsize);
    double** vel = (double**) malloc(sizeof (double*)*popsize);
    double** one_vel = (double**) malloc(sizeof (double*)*popsize);
    double** zero_vel = (double**) malloc(sizeof (double*)*popsize);
    for (int i = 0; i < popsize; i++) {
        x[i] = (char*) malloc(sizeof (char)*malloc_size);
        xpbest[i] = (char*) malloc(sizeof (char)*malloc_size);
        vel[i] = (double*) malloc(sizeof (double)*malloc_size);
        one_vel[i] = (double*) malloc(sizeof (double)*malloc_size);
        zero_vel[i] = (double*) malloc(sizeof (double)*malloc_size);
    }
    double* fx = (double *) malloc(sizeof (double)*popsize);
    double* pbest = (double *) malloc(sizeof (double)*popsize);
    char* xgbest = (char *) malloc(sizeof (char)*malloc_size);

    srand(time(0));
    for (int tt = 0; tt < 10; tt++) {
        for (int i = 0; i < popsize; i++) {
            for (int j = 0; j < malloc_size; j++) {
                xpbest[i][j] = x[i][j] = rand() % 2;
                vel[i][j] = ((double) rand() / (RAND_MAX)) - 0.5;
                one_vel[i][j] = ((double) rand() / (RAND_MAX)) - 0.5;
                zero_vel[i][j] = ((double) rand() / (RAND_MAX)) - 0.5;
            }
        }

        for (int i = 0; i < popsize; i++) {
            All_Model* m = static_cast<All_Model*>(master->clone());
            pbest[i] = fx[i] = m->fx_function_solve(malloc_size, x[i], false);
            if (m)
                delete m;
        }

        double w1 = configs["WEIGHT"];
        double c1 = configs["C1"];
        double c2 = configs["C2"];
        int maxiter = configs["ITERATION"];
        double vmax = configs["VMAX"];

        int l;
        double gbest;
        double gg;

        minimum(l, gbest, popsize, fx);
        memcpy(xgbest, x[l], malloc_size);

        for (int iter = 1; iter <= maxiter; iter++) {
            double w = 0.5;
            for (int i = 0; i < popsize; i++) {
                All_Model* m = static_cast<All_Model*>(master->clone());
                fx[i] = m->fx_function_solve(malloc_size, x[i], false);
                if (m) {
                    delete m;
                }
                if (fx[i] < pbest[i]) {
                    pbest[i] = fx[i];
                    memcpy(xpbest[i], x[i], malloc_size);
                }
            }
            minimum(l, gg, popsize, fx);
            if (gbest > gg) {
                gbest = gg;
                memcpy(xgbest, x[l], malloc_size);
            }

            double c3 = c1 * ((double) rand() / (RAND_MAX));
            double dd3 = c2 * ((double) rand() / (RAND_MAX));

            for (int i = 0; i < popsize; i++) {
                for (int j = 0; j < malloc_size; j++) {
                    double oneadd = 0, zeroadd = 0;
                    if (xpbest[i][j] == 0) {
                        oneadd = oneadd - c3;
                        zeroadd = zeroadd + c3;
                    } else {
                        oneadd = oneadd + c3;
                        zeroadd = zeroadd - c3;
                    }
                    if (xgbest[j] == 0) {
                        oneadd = oneadd - dd3;
                        zeroadd = zeroadd + dd3;
                    } else {
                        oneadd = oneadd + dd3;
                        zeroadd = zeroadd - dd3;
                    }
                    one_vel[i][j] = (w1 * one_vel[i][j]) + oneadd;
                    zero_vel[i][j] = (w1 * zero_vel[i][j]) + zeroadd;
                    if (fabs(vel[i][j]) > vmax) {
                        zero_vel[i][j] = vmax * sign(zero_vel[i][j]);
                        one_vel[i][j] = vmax * sign(one_vel[i][j]);
                    }
                    if (x[i][j] == 1) {
                        vel[i][j] = zero_vel[i][j];
                    } else {
                        vel[i][j] = one_vel[i][j];
                    }
                    if (((double) rand() / (RAND_MAX)) < logsig(vel[i][j])) {
                        x[i][j] = !x[i][j];
                    } else {
                        x[i][j] = x[i][j];
                    }
                }
            }
        }
        if (Gbest1 > gbest) {
            Gbest1 = gbest;
        }
        double temp = 0;
        for (int i = 0; i < popsize; i++) {
            temp += pbest[i];
        }
        temp /= popsize;
        if (Pbest1 > temp) {
            Pbest1 = temp;
        }
    }
    printf("%s : %lf\n", file_name, Gbest1);

    // master->display();
    double best_y_1 = master->fx_function_solve(malloc_size, xgbest, true);
    // master->display();
    master->ls_analyze();

    for (int tt = 0; tt < 10; tt++) {
        for (int i = 0; i < popsize; i++) {
            for (int j = 0; j < malloc_size; j++) {
                xpbest[i][j] = x[i][j] = rand() % 2;
                vel[i][j] = ((double) rand() / (RAND_MAX)) - 0.5;
                one_vel[i][j] = ((double) rand() / (RAND_MAX)) - 0.5;
                zero_vel[i][j] = ((double) rand() / (RAND_MAX)) - 0.5;
            }
        }

        for (int i = 0; i < popsize; i++) {
            All_Model* m = static_cast<All_Model*>(master->clone());
            pbest[i] = fx[i] = m->fx_function_solve_2(malloc_size, x[i], false);
            if (m)
                delete m;
        }

        double w1 = configs["WEIGHT"];
        double c1 = configs["C1"];
        double c2 = configs["C2"];
        int maxiter = configs["ITERATION"];
        double vmax = configs["VMAX"];

        int l;
        double gbest;
        double gg;

        minimum(l, gbest, popsize, fx);
        memcpy(xgbest, x[l], malloc_size);

        for (int iter = 1; iter <= maxiter; iter++) {
            double w = 0.5;
            for (int i = 0; i < popsize; i++) {
                All_Model* m = static_cast<All_Model*>(master->clone());
                fx[i] = m->fx_function_solve_2(malloc_size, x[i], false);
                if (m) {
                    delete m;
                }
                if (fx[i] < pbest[i]) {
                    pbest[i] = fx[i];
                    memcpy(xpbest[i], x[i], malloc_size);
                }
            }
            minimum(l, gg, popsize, fx);
            if (gbest > gg) {
                gbest = gg;
                memcpy(xgbest, x[l], malloc_size);
            }

            double c3 = c1 * ((double) rand() / (RAND_MAX));
            double dd3 = c2 * ((double) rand() / (RAND_MAX));

            for (int i = 0; i < popsize; i++) {
                for (int j = 0; j < malloc_size; j++) {
                    double oneadd = 0, zeroadd = 0;
                    if (xpbest[i][j] == 0) {
                        oneadd = oneadd - c3;
                        zeroadd = zeroadd + c3;
                    } else {
                        oneadd = oneadd + c3;
                        zeroadd = zeroadd - c3;
                    }
                    if (xgbest[j] == 0) {
                        oneadd = oneadd - dd3;
                        zeroadd = zeroadd + dd3;
                    } else {
                        oneadd = oneadd + dd3;
                        zeroadd = zeroadd - dd3;
                    }
                    one_vel[i][j] = (w1 * one_vel[i][j]) + oneadd;
                    zero_vel[i][j] = (w1 * zero_vel[i][j]) + zeroadd;
                    if (fabs(vel[i][j]) > vmax) {
                        zero_vel[i][j] = vmax * sign(zero_vel[i][j]);
                        one_vel[i][j] = vmax * sign(one_vel[i][j]);
                    }
                    if (x[i][j] == 1) {
                        vel[i][j] = zero_vel[i][j];
                    } else {
                        vel[i][j] = one_vel[i][j];
                    }
                    if (((double) rand() / (RAND_MAX)) < logsig(vel[i][j])) {
                        x[i][j] = !x[i][j];
                    } else {
                        x[i][j] = x[i][j];
                    }
                }
            }
        }
        if (Gbest1 > gbest) {
            Gbest1 = gbest;
        }
        double temp = 0;
        for (int i = 0; i < popsize; i++) {
            temp += pbest[i];
        }
        temp /= popsize;
        if (Pbest1 > temp) {
            Pbest1 = temp;
        }
    }
    printf("%s : %lf\n", file_name, Gbest1);

    double best_y_2 = master->fx_function_solve_2(malloc_size, xgbest, true);
    master->display();

    printf("Best Result SS: %lf\n", best_y_1);
    printf("Best Result LS: %lf\n", best_y_2);

    for (int i = 0; i < popsize; i++) {
        free(x[i]);
        free(xpbest[i]);
        free(vel[i]);
        free(one_vel[i]);
        free(zero_vel[i]);
    }
    free(x);
    free(xpbest);
    free(vel);
    free(one_vel);
    free(zero_vel);
    free(fx);
    free(pbest);
    free(xgbest);

    if (master) {
        delete master;
    }

    return 0;
}
