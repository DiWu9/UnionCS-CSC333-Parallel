#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//author: Di Wu
//to compile: mpicc -std=c99 -Wall MPI_vector_add.c -o MPI_vector_add
//to run (simple): mpirun -np (#processor) ./MPI_vector_add (array size)

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

	// compute block distrubition factors
	int avg_block_size = vector_size / comm_sz;
	int remainder = vector_size % comm_sz;
	int block_size = my_rank < remainder ? avg_block_size + 1 : avg_block_size;

	// initialize block pointer
	double *block_vec1 = calloc(avg_block_size + 1, sizeof(double));
	double *block_vec2 = calloc(avg_block_size + 1, sizeof(double));

	if (my_rank == 0)
	{
		double *vec1 = calloc(vector_size, sizeof(double));
		double *vec2 = calloc(vector_size, sizeof(double));

		// randomize vectors
		vec1 = randomize_vector(vec1, vector_size);
		vec2 = randomize_vector(vec2, vector_size);

		for (int offset = 0; offset < block_size; offset++)
		{
			*(block_vec1 + offset) = *(vec1 + offset);
			*(block_vec2 + offset) = *(vec2 + offset);
		}

		for (int i = 1; i < comm_sz; i++)
		{
			// block-wise distribute data
			int start_index = i <= remainder ? (avg_block_size + 1) * i : avg_block_size * i + remainder;
			int block_size = i < remainder ? avg_block_size + 1 : avg_block_size;
			MPI_Send(vec1 + start_index, block_size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			MPI_Send(vec2 + start_index, block_size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Recv(block_vec1, block_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(block_vec2, block_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	// do vector addition
	double *block_sum = calloc(block_size, sizeof(double));
	double *vec_sum = calloc(vector_size, sizeof(double));

	for (int j = 0; j < block_size; j++)
	{
		*(block_sum + j) = *(block_vec1 + j) + *(block_vec2 + j);
	}

	if (my_rank != 0)
	{
		MPI_Send(block_sum, block_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	else
	{
		// copy root task result to vec_sum
		int global_offset = 0;
		for (int offset = 0; offset < block_size; offset++)
		{
			*(vec_sum + offset) = *(block_sum + offset);
			global_offset++;
		}
		// copy non-root tasks result to vec_sum
		int block_q_size = 0;
		for (int q = 1; q < comm_sz; q++)
		{
			block_q_size = q < remainder ? avg_block_size + 1 : avg_block_size;
			MPI_Recv(block_sum, block_q_size, MPI_DOUBLE, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for (int offset = 0; offset < block_q_size; offset ++)
			{
				* (vec_sum + global_offset) = * (block_sum + offset);
				global_offset++;
			}
		}
		printf("The vector sum is ");
		print_vector(vec_sum, vector_size);
	}

	MPI_Finalize();

	// time end, print execution time
	clock_t endTime = clock();
	double elapsed = (double)(endTime - startTime) * 1000.0 / CLOCKS_PER_SEC;
	printf("Execution time: %f ms.\n", elapsed);
	
	//please no MPI stuff after this line
	return 0;
}
