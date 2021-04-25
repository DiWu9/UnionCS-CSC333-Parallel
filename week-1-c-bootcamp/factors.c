#include <stdio.h>
#include <time.h>
#include <math.h>
#include <limits.h>

/*
author: Di Wu
date: 04/01/2021
*/

int main(int argc, char *argv[])
{

  //printf ("sizeof unsigned long: %lu",sizeof(unsigned long));
  //printf ("sizeof unsigned long long: %lu",sizeof(unsigned long long));
  unsigned long long bignumber = 73856093;
  unsigned long i;
  unsigned long max_factor = 0;

  clock_t startTime = clock();
  for (i = 3; i < sqrt(bignumber); i+=2)
  {
    if ((bignumber % i) == 0)
    {
      max_factor = max_factor < i ? i : max_factor;
    }
  }
  if (max_factor > 0)
  {
    printf("The number not a prime number and the largest factor is %ld ",max_factor);
  }
  else
  {
    printf("The number is a prime number ");
  }
  clock_t endTime = clock();
  double elapsed = (double)(endTime - startTime) * 1000.0 / CLOCKS_PER_SEC;
  printf("that took %f ms\n", elapsed);
}
