//
// Created by RuoHao Li on 6/17/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include "iostream"

int main(int argc, char *argv[]) {
    double niter = 10000000;
    double x, y;
    int i;
    int count = 0;
    double z;
    double pi;
    srand(time(NULL));

    time_t tstart, tend;
    tstart = time(0);

    //main loop
    for (i = 0; i < niter; ++i) {
        //get random points
        x = (double) random() / RAND_MAX;
        y = (double) random() / RAND_MAX;
        z = sqrt((x * x) + (y * y));
        //check to see if point is in unit circle
        if (z <= 1) {
            ++count;
        }
    }
    pi = ((double) count / (double) niter) * 4.0;          //p = 4(m/n)
    tend = time(0);
    std::cout << "It took " << difftime(tend, tstart) << " second(s)." << std::endl;

    printf("Pi: %f\n误差：%lf\n", pi, fabs(3.141592 - pi));

    return 0;
}