#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*
vectors.c for week 1 bootcamp
author: John Rieffel
modified by: Di Wu
*/

//argc is the argument count from the commandline
//argc is always at least 1, because the excutable
//is an argument
//
//each argument is held in a separate string.
//argv is an *array* of strings

double *randomize_vector(double *vec, int size)
{
  int index;
  for (index = 0; index < size; index++)
  {
    vec[index] = drand48(); // non-negative, double-precision, uniformly distributed over the interval [0.0 , 1.0].
  }
  return vec;
}

double *randomize_vector_p(double *vec, int size)
{
  double *p;
  double *vec_end = vec + size;
  for (p = vec; p < vec_end; p++)
  {
    *p = drand48();
  }
  return vec;
}

double *vector_addition(double *sum_vec, double *vec1, double *vec2, int size)
{
  int offset;
  for (offset = 0; offset < size; offset++)
  {
    *(sum_vec + offset) = *(vec1 + offset) + *(vec2 + offset);
  }
  return sum_vec;
}

double dot_product(double *vec1, double *vec2, int size)
{
  int offset;
  double product = 0;
  for (offset = 0; offset < size; offset++)
  {
    product += (*(vec1 + offset)) * (*(vec2 + offset));
  }
  return product;
}

void print_vector(double *vec, int size)
{
  int index;
  for (index = 0; index < size; index++)
  {
    printf("%f ", vec[index]);
  }
  printf("\n");
  return;
}

void print_vector_p(double *vec, int size)
{
  double *p;
  double *vec_end = vec + size;
  for (p = vec; p < vec_end; p++)
  {
    printf("%f ", *p);
  }
  printf("\n");
  return;
}

int main(int argc, char *argv[])
{
  clock_t startTime = clock();

  int SIZE = 0;

  if ((argc < 2) || (argc > 3))
  {
    printf("usage: vectors <size> or vectors <size> <seed>\n ");
    exit(1);
  }
  else
  {
    SIZE = atoi(argv[1]); //atoi converts a string to an int
    printf("size: %d\n", SIZE);
    if (argc == 3)
      srand48(atoi(argv[2])); // initialization entry points
    else
      srand48(time(NULL));
  }

  //calloc, unlike malloc, zeros the allocated memory
  double *vector1 = calloc(SIZE, sizeof(double));
  double *vector2 = calloc(SIZE, sizeof(double));
  double *vector_sum = calloc(SIZE, sizeof(double));
  double product;

  vector1 = randomize_vector_p(vector1, SIZE);
  vector2 = randomize_vector_p(vector2, SIZE);
  vector_sum = vector_addition(vector_sum, vector1, vector2, SIZE);
  product = dot_product(vector1, vector2, SIZE);

  //print_vector(vector1, SIZE);
  //print_vector(vector2, SIZE);
  //print_vector(vector_sum, SIZE);
  printf("Dot Product: %f. \n", product);
  clock_t endTime = clock();
  double elapsed = (double)(endTime - startTime) * 1000.0 / CLOCKS_PER_SEC;
  printf("Execution time: %f ms.\n", elapsed);
}
