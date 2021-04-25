#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Di Wu
//to compile: mpicc -std=c99 -Wall MPI_Vector_Mean.c -o MPI_Vector_Mean
//to run (simple): mpirun -np (#processor) ./MPI_Vector_Mean

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

int main(int argc, char *argv[])
{
	int comm_sz; /* Number of processes    */
	int my_rank; /* My process rank        */
	//Don't put any MPI stuff before this line
	MPI_Init(&argc, &argv); //sets up the MPI. Always this line!

	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	double start = MPI_Wtime();
	double end;

	int global_size = argc > 1 ? atoi(argv[1]) : 12;
	int remainder = global_size % comm_sz;
	int local_size = my_rank < remainder ? global_size / comm_sz + 1 : global_size / comm_sz;
	double *local_array = malloc(local_size * sizeof(double));
	randomize_vector(local_array, local_size);
	// print_vector(local_array, local_size);

	double local_sum = 0;
	double global_sum = 0;
	for (int i = 0; i < local_size; i++)
	{
		local_sum += local_array[i];
	}
	MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (my_rank == 0)
	{
		double avg = global_sum / global_size;
		printf("The average of the %d-vector is %.2f.\n", global_size, avg);
		end = MPI_Wtime();
		printf("The root task took %.2f ms to execute.\n", (end - start) * 1000);
	}

	MPI_Finalize();
	//please no MPI stuff after this line
	return 0;
}
