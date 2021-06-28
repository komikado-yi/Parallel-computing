//
// Created by RuoHao Li on 6/16/21.
//

#include "mpi.h"
#include <stdio.h>
#include <math.h>

double f(double x) {
    return cos(x);
}

int main(int argc, char *argv[]) {
    double start = -1, end = 1;//上下限
    int n = 100;//每一个进程的一小块分成多少块
    double w;//每一块的宽度
    int i, myrank, size;
    double startwtime, endwtime;
    double s = 0.0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    w = (end - start) / size / n;  //每一个进程都参与计算
    startwtime = MPI_Wtime();

    double eachprocessval = 0;
    for (i = 0; i < n; i++) {
        eachprocessval += f((start * 2 + (myrank * n + i) * w * 2 + w) / 2) * w;
    }
    //最后一个进程不仅计算还负责汇总
    if (myrank != size - 1) {
        MPI_Send(&eachprocessval, 1, MPI_DOUBLE, size - 1, 99, MPI_COMM_WORLD);
    } else {
        s = eachprocessval;
        double tempeachans;
        for (i = 0; i < size - 1; i++) {
            MPI_Recv(&tempeachans, 1, MPI_DOUBLE, i, 99, MPI_COMM_WORLD, &status);
            s += tempeachans;
        }
        endwtime = MPI_Wtime();
        printf("计算进程数：%d\n积分值：%lf\n误差：%lf\n", size, s, fabs(1.6829419696158 - s));
        printf("所用时间：%f秒\n", endwtime - startwtime);
    }
    MPI_Finalize();
    return 0;
}

