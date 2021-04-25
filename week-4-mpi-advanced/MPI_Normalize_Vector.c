#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Di Wu
// to compile: mpicc -std=c99 -Wall MPI_Normalize_Vector.c -o MPI_Normalize_Vector
// to run (simple): mpirun -np (#processor) ./MPI_Normalize_Vector

double *randomize_vector(double *vec, int size)
{
    int index;
    for (index = 0; index < size; index++)
    {
        vec[index] = drand48(); // non-negative, double-precision, uniformly distributed over the interval [0.0 , 1.0].
    }
    return vec;
}

void print_vector(double *vec, int size)
{
    int index;
    for (index = 0; index < size; index++)
    {
        printf("%.2f ", vec[index]);
    }
    printf("\n");
    return;
}

double vector_sum_square(double *vec, int size)
{
    double square_sum = 0;
    for (int index = 0; index < size; index++)
    {
        square_sum += pow(vec[index], 2.0);
    }
    return square_sum;
}

int main(int argc, char *argv[])
{
    int vector_size = argc > 1 ? atoi(argv[1]) : 12; /* default array size is 12 */
    int comm_sz;                                     /* Number of processes    */
    int my_rank;                                     /* My process rank        */

    //Don't put any MPI stuff before this line
    MPI_Init(&argc, &argv); //sets up the MPI. Always this line!

    /* Get the number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    /* Get my rank among all the processes */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    double start;
    double end;
    int slice_size = vector_size / comm_sz;
    double *vector = malloc(vector_size * sizeof(double));
    double *slice_vector = malloc(slice_size * sizeof(double));

    // root task randomizes vector
    if (my_rank == 0)
    {
        // start WTiming
        start = MPI_Wtime();
        vector = randomize_vector(vector, vector_size);
    }

    MPI_Scatter(vector, slice_size, MPI_DOUBLE, slice_vector, slice_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // compute local square sum
    double local_sum = vector_sum_square(slice_vector, slice_size);
    double global_sum = 0;
    // use Allreduce to sum local squares, now all nodes have global sum
    MPI_Allreduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    double magnitude = sqrt(global_sum);

    for (int k = 0; k < slice_size; k++)
    {
        slice_vector[k] = slice_vector[k] / magnitude;
    }

    // gather norm vectors
    double *vector_norm = malloc(vector_size * sizeof(double));
    MPI_Gather(slice_vector, slice_size, MPI_DOUBLE, vector_norm, slice_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // check if magnitude is 1
    //double check_magnitude = compute_magnitude(vector_norm, vector_size, comm_sz, my_rank);

    if (my_rank == 0)
    {
        printf("The vector norm is: ");
        print_vector(vector_norm, vector_size);
        //printf("The magnitude is %.2f.\n", check_magnitude);
        end = MPI_Wtime();
        printf("The root task took %.2f ms to execute.\n", (end - start) * 1000);
    }

    MPI_Finalize();
    //please no MPI stuff after this line
    return 0;
}
