#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Di Wu
// to compile: mpicc -std=c99 -Wall MPI_Normalize_Vector_Using_Scatterv.c -o MPI_Normalize_Vector_Scatterv
// to run (simple): mpirun -np (#processor) ./MPI_Normalize_Vector_Scatterv

double *randomize_vector(double *vec, int size)
{
    int index;
    for (index = 0; index < size; index++)
    {
        vec[index] = (double)1; //drand48(); // non-negative, double-precision, uniformly distributed over the interval [0.0 , 1.0].
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

void print_vector_int(int *vec, int size)
{
    for (int index = 0; index < size; index++)
    {
        printf("%d ", vec[index]);
    }
    printf("\n");
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
    int remainder = vector_size % comm_sz;
    double *vector = malloc(vector_size * sizeof(double));

    // root task randomizes vector
    if (my_rank == 0)
    {
        // start WTiming
        start = MPI_Wtime();
        vector = randomize_vector(vector, vector_size);
    }

    // Scatterv Prep
    int counts[comm_sz];
    int displs[comm_sz];
    int count = 0;
    int disp = 0;

    for (int i = 0; i < comm_sz; i++)
    {
        count = i < remainder ? slice_size + 1 : slice_size;
        *(counts + i) = count;
        *(displs + i) = disp;
        disp += count;
    }

    double *recv_vector = malloc(counts[my_rank] * sizeof(double));
    MPI_Scatterv(vector, counts, displs, MPI_DOUBLE, recv_vector, counts[my_rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    printf("Node %d have slice: ", my_rank);
    print_vector(recv_vector, counts[my_rank]);

    double square_sum = vector_sum_square(recv_vector, counts[my_rank]);
    double *square_sums = malloc(comm_sz * sizeof(double));
    double magnitude = 0;
    MPI_Gather(&square_sum, 1, MPI_DOUBLE, square_sums, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (my_rank == 0)
    {
        for (int j = 0; j < comm_sz; j++)
        {
            magnitude += *(square_sums + j);
        }
        magnitude = sqrt(magnitude);
    }
    MPI_Bcast(&magnitude, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int k = 0; k < counts[my_rank]; k++)
    {
        recv_vector[k] = recv_vector[k] / magnitude;
    }

    // gather norm vectors
    double *vector_norm = malloc(vector_size * sizeof(double));
    MPI_Gatherv(recv_vector, counts[my_rank], MPI_DOUBLE, vector_norm, counts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // check if magnitude is 1
    // double check_magnitude = compute_magnitude(vector_norm, vector_size, comm_sz, my_rank);

    if (my_rank == 0)
    {
        printf("The vector norm is: ");
        print_vector(vector_norm, vector_size);
        end = MPI_Wtime();
        printf("The root task took %.2f ms to execute.\n", (end - start) * 1000);
    }

    MPI_Finalize();
    //please no MPI stuff after this line
    return 0;
}