#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//author: Di Wu
//to compile: mpicc -std=c99 -Wall MPI_vector_dot.c -o MPI_vector_dot
//to run (simple): mpirun -np (#processor) ./MPI_vector_dot (array size)

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
		printf("%f ", vec[index]);
	}
	printf("\n");
	return;
}

int main(int argc, char *argv[])
{
	// start clock
	clock_t startTime = clock();

	int vector_size = argc > 1 ? atoi(argv[1]) : 10; /* default array size is 10 */
	int comm_sz;									 /* Number of processes    */
	int my_rank;									 /* My process rank        */
	//Don't put any MPI stuff before this line
	MPI_Init(&argc, &argv); //sets up the MPI. Always this line!

	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	double *vec_head1 = calloc(vector_size, sizeof(double));
	double *vec_head2 = calloc(vector_size, sizeof(double));

	if (my_rank == 0)
	{
		// initialize vectors
		double *vector1 = calloc(vector_size, sizeof(double));
		double *vector2 = calloc(vector_size, sizeof(double));

		// randomize two vectors of same size
		vector1 = randomize_vector(vector1, vector_size);
		vector2 = randomize_vector(vector2, vector_size);

		vec_head1 = vector1;
		vec_head2 = vector2;

		for (int i = 1; i < comm_sz; i++)
		{
			MPI_Send(vector1, vector_size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			MPI_Send(vector2, vector_size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Recv(vec_head1, vector_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(vec_head2, vector_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	// cyclic dot product
	double local_dot = 0;
	double global_dot;

	for (int i = my_rank; i < vector_size; i += comm_sz)
	{
		local_dot += *(vec_head1 + i) * *(vec_head2 + i);
	}

	if (my_rank != 0)
	{
		MPI_Send(&local_dot, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	else
	{
		global_dot = local_dot;
		for (int q = 1; q < comm_sz; q++)
		{
			MPI_Recv(&local_dot, 1, MPI_DOUBLE, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			global_dot += local_dot;
		}
		printf("The dot product is %f\n", global_dot);
	}

	MPI_Finalize();

	// time end, print execution time
	clock_t endTime = clock();
	double elapsed = (double)(endTime - startTime) * 1000.0 / CLOCKS_PER_SEC;
	printf("Execution time: %f ms.\n", elapsed);

	//please no MPI stuff after this line
	return 0;
}
