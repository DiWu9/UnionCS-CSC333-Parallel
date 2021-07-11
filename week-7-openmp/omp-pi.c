#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
/*
name: omp-pi.c
author: Di Wu
description: Use Riemann Sum to estimate the value of pi
*/

int main(int argc, char *argv[])
{
    int nthreads = 1;
    double N = 1000;
    if (argc > 1)
        nthreads = strtol(argv[1], NULL, 10);
    if (argc > 2)
        N = atoi(argv[2]); //atoi converts a string to an intm
    double startTime = omp_get_wtime();
    double dx = 1 / N;
    double sum = 0;

    #pragma omp parallel for num_threads(nthreads) reduction(+ : sum)
    for (int i = 0; i < N; i++)
    {
        double xval = i * dx;
        double h = sqrt(1 - pow(xval, 2));
        sum += h * dx;
        //printf("Thread %d get i value %d.\n", omp_get_thread_num(), i);
    }

    printf("The estimated value of Pi is %f. \n", sum * 4);

    double endTime = omp_get_wtime();
    double elapsed = (endTime - startTime) * 1000.0;
    printf("Estimating Pi took %f ms\n", elapsed);
}