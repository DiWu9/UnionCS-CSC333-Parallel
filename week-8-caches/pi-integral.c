#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
/*
name: pi-integral.c
author: John Rieffel
description:

calculate pi by taking the integral of a  quarter of a circle with radius 2

The Leibnitz Formula states that

arctan(1) = pi/4

and we know that arctan's derivative is:

1/1+x^2

so if we take the integral of that, we can approximate pi:

integral-sign (4/1+x^2) -> pi

which we can approximate by summing a bunch of rectangles

sum (from 0 to N) of F(x)*deltaX

where deltaX is the width of the rectangle and F(x)


*/

int main(int argc, char *argv[])
{
  static long num_steps = 1000000;
  int nthreads = 1;
  if (argc > 1)
	 num_steps = strtol(argv[1],NULL,10);
  if (argc > 2)
	 nthreads = strtol(argv[2],NULL,10);

   long i;
   long double x, pi, sum = 0.0;
   long double step;

   step = 1.0/(double) num_steps;
   for (i=0;i< num_steps; i++){
     x = (i+0.5)*step;
     sum = sum + 4.0/(1.0+x*x);
   }
   pi = step * sum;

   printf("pi is %.100Lf\n", pi);



}
