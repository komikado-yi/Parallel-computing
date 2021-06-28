//
/* Created by RuoHao Li on 6/17/21.
    除了0号结点，剩下的都做计算(投点、收集、计数)，0号负责记录一共投的次数与落在圆内的次数 */
//

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>

#define SEED 35791246

int main(int argc, char *argv[]) {
    long niter = 1000000;
    int myid;                       //holds process's rank id
    double x, y;                     //x,y value for the random coordinate
    int i, count = 0;                 //Count holds all the number of how many good coordinates
    double z;                       //Used to check if x^2+y^2<=1
    double pi;                      //holds approx value of pi
    int nodenum;
    double tic, toc;
    MPI_Request req[2];
    MPI_Status sta[2];

    MPI_Init(&argc, &argv);                 //Start MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);           //get rank of node's process
    MPI_Comm_size(MPI_COMM_WORLD, &nodenum);
    int recieved[nodenum];
    long recvniter[nodenum];
    srand(SEED + myid);                       //Give rand() a seed value. Needs to be different on each node

    tic = MPI_Wtime();

    if (myid != 0) {
        for (i = 0; i < niter; ++i)                  //main loop
        {
            x = ((double) rand()) / RAND_MAX;           //gets a random x coordinate
            y = ((double) rand()) / RAND_MAX;           //gets a random y coordinate
            z = sqrt(x * x + y * y);                  //Checks to see if number in inside unit circle
            if (z <= 1) {
                count++;                //if it is, consider it a valid random point
            }
        }
        for (i = 0; i < nodenum; ++i) {
            MPI_Isend(&count,    //pointer to the send buffer
                      1, //Number of elements in the send buffer
                      MPI_INT,   //kind of data thats being sent
                      0, //destination rank
                      1, //message tag to help identify a specific message or type of message, specified/defined by the application
                      MPI_COMM_WORLD,    //MPI communicator
                      req);
            MPI_Wait(req, sta);
            MPI_Isend(&niter,
                      1,
                      MPI_LONG,
                      0,
                      2,
                      MPI_COMM_WORLD,
                      req);
            MPI_Wait(req, sta);
        }
    } else if (myid == 0) {
        for (i = 0; i < nodenum; ++i) {
            MPI_Irecv(&recieved[i],  //buffer to store the reveived data
                      nodenum,   //maximum elements in the receive buffer
                      MPI_INT,   //appropriate type for the data to be revieved
                      MPI_ANY_SOURCE,    //rank number that the message will be sent from
                      1, //a label to identify a particular message, is defined by an application
                      MPI_COMM_WORLD,    //name of the MPI communicator
                      &req[1]);    //status variable
            MPI_Wait(&req[1], &sta[1]);
            MPI_Irecv(&recvniter[i],
                      nodenum,
                      MPI_LONG,
                      MPI_ANY_SOURCE,
                      2,
                      MPI_COMM_WORLD,
                      &req[1]
            );
            MPI_Wait(&req[1], &sta[1]);
        }
    }

    if (myid == 0)                      //if root process
    {
        int finalcount = 0;
        long finalniter = 0;
        for (i = 0; i < nodenum; ++i) {
            finalcount += recieved[i];
            finalniter += recvniter[i];
        }

        pi = ((double) finalcount / (double) finalniter) * 4.0;               //p = 4(m/n)

        toc = MPI_Wtime();

        printf("Pi: %f\n误差：%lf\n", pi, fabs(3.141592 - pi));             //Print the calculated value of pi
        printf("所用时间：%f秒\n", toc - tic);


    }

    MPI_Finalize();                     //Close the MPI instance
    return 0;
}