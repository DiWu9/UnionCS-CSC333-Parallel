#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
/*
name: matrices.c
author: Di Wu
description: 

parallelizing matrix addition and transpose using OpenMP
*/

double *randomize_matrix(double *mat, int row, int col)
{
  // THIS WILL BE YOUR RANDOM SEED
  // YES IT IS NON-RANDOM - Read the lab!
  unsigned short xi[3] = {1, 9, 99};
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      mat[i * col + j] = erand48(xi);
    }
  }
  return mat;
}

double *add_matrix(double *mat1, double *mat2, int row, int col, int nthreads, int chunk)
{
  double *sum = calloc(row * col, sizeof(double));
# pragma omp parallel for schedule(static, chunk) num_threads(nthreads)
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      sum[i * col + j] = mat1[i * col + j] + mat2[i * col + j];
    }
  }
  return sum;
}

double *transpose_matrix(double *mat, int row, int col, int nthreads, int chunk)
{
  double *trans = calloc(col * row, sizeof(double));
# pragma omp parallel for schedule(static, chunk) num_threads(nthreads)
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      trans[j * col + i] = mat[i * col + j];
    }
  }
  return trans;
}

void print_matrix(double *mat, int row, int col)
{
  int r, c;

  for (r = 0; r < row; r++)
  {
    for (c = 0; c < col; c++)
    {
      printf("%f ", mat[r * col + c]);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[])
{

  int ROWS = 0;
  int COLS = 0;
  int nthreads = 1;
  int chunk = 1;

  if (argc < 3)
  {
    printf("usage: matrices <rows> <cols> <nthreads> <chunk>\n ");
    exit(1);
  }
  else
  {
    ROWS = atoi(argv[1]);
    COLS = atoi(argv[2]);
    if (argc >= 4)
    {
      nthreads = atoi(argv[3]);
      if (argc == 5)
      {
        chunk = atoi(argv[4]);
      }
      else
      {
        printf("usage: matrices <rows> <cols> <nthreads> <chunk>\n ");
        exit(1);
      }
    }
    printf("Rows: %d, Cols: %d\n", ROWS, COLS);
  }
  
  double *MAT1 = calloc(ROWS * COLS, sizeof(double));
  double *MAT2 = calloc(ROWS * COLS, sizeof(double));

  MAT1 = randomize_matrix(MAT1, ROWS, COLS);
  MAT2 = randomize_matrix(MAT2, ROWS, COLS);
  /*
  printf("Matrix 1: \n");
  print_matrix(MAT1, ROWS, COLS);
  printf("Matrix 2: \n");
  print_matrix(MAT2, ROWS, COLS);
  */

  // start timing
  double startTime = omp_get_wtime();

  // matrix addition
  double *MAT_SUM = calloc(ROWS * COLS, sizeof(double));
  MAT_SUM = add_matrix(MAT1, MAT2, ROWS, COLS, nthreads, chunk);
  /*
  printf("Matrix Sum: \n");
  print_matrix(MAT_SUM, ROWS, COLS);
  */

  double endTime = omp_get_wtime();
  double elapsed = (endTime - startTime);
  printf("Matrix Addition took %f s\n", elapsed);
  startTime = endTime;

  // matrix transpose
  double *MAT_TRAN = calloc(ROWS * COLS, sizeof(double));
  MAT_TRAN = transpose_matrix(MAT1, ROWS, COLS, nthreads, chunk);
  /*
  printf("Matrix Transpose: \n");
  print_matrix(MAT_TRAN, ROWS, COLS);
  */

  endTime = omp_get_wtime();
  elapsed = (endTime - startTime);
  printf("Matrix Transpose took %f s\n", elapsed);

  //free(MAT); //don't FREE! C99 takes care of it if allocated like this
  //(instead of via malloc)
}
