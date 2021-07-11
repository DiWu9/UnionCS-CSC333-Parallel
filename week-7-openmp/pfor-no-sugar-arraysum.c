#include <stdio.h>
#include <omp.h>
#include <time.h>

int *normal_vector(int *vec, int size)
{
	int index;
	for (index = 0; index < size; index++)
	{
		vec[index] = 1;
	}
	return vec;
}

int main(int argc, char *argv[])
{
	int PiVals[10] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 4};
	//const int SIZE = 1000000;
	//int *vector1 = calloc(SIZE, sizeof(int));
	//vector1 = normal_vector(vector1, SIZE);
	int nthreads = 1;
	if (argc > 1)
		nthreads = strtol(argv[1], NULL, 10);
	int globalsum = 0;
	double startTime = omp_get_wtime();

	#pragma omp parallel num_threads(nthreads)
	{
		int myid = omp_get_thread_num();
		int threadcount = omp_get_num_threads(); //like, super-high threadcount sheets?
		int localsum = 0;
		for (int i = myid; i < 10; i += threadcount)
		{
			//do something here to accumulate a local sum
			localsum += PiVals[i];
		}
	//do something here to (atomically) aggregate local sums into a global sum
	#pragma omp critical
		globalsum += localsum;
	} //end of pfor pragma
	printf("global sum is %d\n", globalsum);
	double endTime = omp_get_wtime();
	double elapsed = (endTime - startTime) * 1000.0;
	printf("Array sum took %f ms\n", elapsed);
}