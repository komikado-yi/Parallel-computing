//
// Created by RuoHao Li on 6/17/21.
//

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>
#include <ctime>

int main(int argc, char *argv[]) {
    int niter = 100000;
    int myid;                       //holds process's rank id
    double x, y;                     //x,y value for the random coordinate
    int i;
    int count = 0;                //Count holds all the number of how many good coordinates
    double z;                       //Used to check if x^2+y^2<=1
    double pi;                      //holds approx value of pi
    int reducedcount;                   //total number of "good" points from all nodes
    int reducedniter;                   //total number of ALL points from all nodes
    double tic, toc;


    MPI_Init(&argc, &argv);                 //Start MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);           //get rank of node's process

    tic = MPI_Wtime();

/* Everyone can do the work */

    for (i = 0; i < niter; ++i)                  //main loop
    {
        srand48(time(NULL) + myid);               //Give rand() a seed value unique on each node (times are synced)
        x = (double) random() / RAND_MAX;          //gets a random x coordinate
        y = (double) random() / RAND_MAX;          //gets a random y coordinate
        z = sqrt((x * x) + (y * y));              //Checks to see if number in inside unit circle
        if (z <= 1) {
            ++count;                //if it is, consider it a valid random point
        }
    }


    MPI_Reduce(&count,
               &reducedcount,
               1,
               MPI_INT,
               MPI_SUM, //sum all the specified values on all nodes
               0,
               MPI_COMM_WORLD);
    MPI_Reduce(&niter,
               &reducedniter,
               1,
               MPI_INT,
               MPI_SUM,
               0,
               MPI_COMM_WORLD);

    if (myid == 0)                      //if root process
    {
        pi = ((double) reducedcount / (double) reducedniter) * 4.0;               //p = 4(m/n)

        toc = MPI_Wtime();

        printf("Pi: %f\nReduced count: %i\nReduced niter: %d\n误差：%lf\n", pi, reducedcount, reducedniter,
               fabs(3.141592 - pi));
        //Print the calculated value of pi
        printf("所用时间：%f秒\n", toc - tic);

    }

    MPI_Finalize();                     //Close the MPI instance
    return 0;
}