#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

/*
vectors.c for week 1 bootcamp
author: John Rieffel
co-author: Di Wu
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
    vec[index] = drand48();
  }
  return vec;
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
}

void vector_add(double *v1, double *v2, double *result, int size, int nthreads)
{
  //write me with openmp
  #pragma omp parallel for num_threads(nthreads)
  for (int i = 0; i < size; i++)
  {
    result[i] = v1[i] + v2[i];
  }
}

//return the mean of a vector
double vector_mean(double *v1, int size, int nthreads)
{
  double globalsum = 0;
  //write me with openmp
  #pragma omp parallel for num_threads(nthreads) reduction(+ : globalsum)
  for (int i = 0; i < size; i++)
  {
    globalsum += v1[i];
  }
  return globalsum / (double)size;
}

//return the magnitude of a a vector
// (squareroot of sum of squares )
double vector_mag(double *v1, int size, int nthreads)
{
  double sumsquare = 0;
  //write me with openmp
  #pragma omp parallel for num_threads(nthreads) reduction(+ : sumsquare)
  for (int i = 0; i < size; i++)
  {
    sumsquare += pow(v1[i], 2.0);
  }
  return sqrt(sumsquare);
}

//normalize a vector, putting result in outv
void vector_normalize(double *v1, double *outv, int size, int nthreads)
{
  //first a call to vector mag
  double mag = vector_mag(v1, size, nthreads);
  //then an omp for loop that divides evertying in the vector by magnitude
  //warning don't ever omp parallelize loops that call functions that also parallelize
  #pragma omp parallel for num_threads(nthreads)
  for (int i = 0; i < size; i++)
  {
    outv[i] = v1[i] / mag;
  }
}

int main(int argc, char *argv[])
{
  int SIZE = 1000000;
  int nthreads = 1;
  if (argc > 4)
  {
    printf("usage: vectors <procs> <size> or vectors <procs> <size> <seed>\n ");
    exit(1);
  }
  else
  {
    if (argc > 1)
      nthreads = strtol(argv[1], NULL, 10);
    if (argc > 2)
      SIZE = atoi(argv[2]); //atoi converts a string to an int
    if (argc > 3)
      srand48(atoi(argv[3]));
    else
      srand48(time(NULL));
  }
  //printf("threads: %d, vector size: %d\n", nthreads, SIZE);

  //calloc, unlike malloc, zeros the allocated memory
  double *vector1 = calloc(SIZE, sizeof(double));
  double *vector2 = calloc(SIZE, sizeof(double));
  // double *vector3 = calloc(SIZE, sizeof(double));

  vector1 = randomize_vector(vector1, SIZE);
  vector2 = randomize_vector(vector2, SIZE);

  double *outv1 = calloc(SIZE, sizeof(double));
  double *outv2 = calloc(SIZE, sizeof(double));

  double startTime = omp_get_wtime();

  printf("Normalize Vectors: \n");
  vector_normalize(vector1, outv1, SIZE, nthreads);
  // vector_normalize(vector2, outv2, SIZE, nthreads);
  // print_vector(outv1, SIZE);
  // print_vector(outv2, SIZE);

  double endTime = omp_get_wtime();
  double elapsed = (endTime - startTime) * 1000.0;
  printf("Normalize vector took %f ms\n", elapsed);

  /*
  printf("Vector Addition: \n");
  double vec1[9] = {1,1,1,1,1,1,1,1,1};
  double vec2[9] = {2,2,2,2,2,2,2,2,2};
  double *result = calloc(9, sizeof(double));
  vector_add(vec1, vec2, result, 9, nthreads);
  print_vector(result, 9);

  printf("Vector Mean: \n");
  double mean = vector_mean(vec1, 9, nthreads);
  printf("The mean is expected to be 1, the actual is %d. \n", (int) mean);
  //printf("%f\n", sqrt(2.0));
  */
}
