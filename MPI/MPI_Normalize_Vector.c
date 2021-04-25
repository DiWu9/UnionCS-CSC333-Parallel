#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> //for random seed
#include <math.h> //need to compile with -m

//being explicit here to make code more readable
#define ROOT_NODE 0
//

int main(int argc, char *argv[])
{

	int comm_sz; /* Number of processes    */
	int my_rank; /* My process rank        */
	//Don't put any MPI stuff before this line

	srand(time(NULL)); // should only be called once

	double *bigVector;
	double *myVectorSlice;
	int BigVectorSize = 200;

	MPI_Init(&argc, &argv); //sets up the MPI. Always this line!
	int i;

	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	//for now, make these evenly divisible.
	//later, we'll cover ScatterV
	int myVectorSliceSize = BigVectorSize / comm_sz;

	if ((BigVectorSize % comm_sz) != 0)
	{
		printf("Can't do this with this number of processes\n");
	}

	//dynamically allocate a vector to hold our personal slice of the data
	myVectorSlice = malloc(myVectorSliceSize * sizeof(double));

	//have the root node generate a big vector of random numbers
	if (my_rank == 0)
	{
		bigVector = malloc(BigVectorSize * sizeof(double));
		printf("original vector:\n");
		int i;
		for (i = 0; i < BigVectorSize; i++)
		{
			bigVector[i] = (float)rand(); // returns a pseudo-random integer between 0 and RAND_MAXA
										  //then cast as a double.  sloppy but effective
			printf("%f ", bigVector[i]);
		}
	}

	//all processes call this this same
	MPI_Scatter(bigVector, myVectorSliceSize, MPI_DOUBLE, myVectorSlice, myVectorSliceSize, MPI_DOUBLE, ROOT_NODE, MPI_COMM_WORLD);

	//now all processes have a personal block-distributed version of the big vector
	//and each processes' copy is different

	//so now calculate local magnitude
	// correctly (via squares) -- (Matthew was right)
	double sumSquaresOfSlice = 0;

	for (i = 0; i < myVectorSliceSize; i++)
	{
		//acknowledging that two array references is inelegant
		//but will probably be optimized away by a good compiler
		sumSquaresOfSlice += myVectorSlice[i] * myVectorSlice[i];
	}

	double sumAllSquares = 0;
	//now need to do an MPI_Allreduce using Sum
	//this calculates the SUM of every proceses personal sumSquaresOfSlice
	//and then re-distributes it to all the processes
	//this is faster than a simple Reduce-Sum to node 0 followed by a Broadcast from node 0
	// this is like a butterfly!
	MPI_Allreduce(&sumSquaresOfSlice, &sumAllSquares, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
	//now all the processes have a correct value of sumAllSquares

	//so all we need to do is get the sqrt, and then divide every element in our personal slice by that value
	double globalMag = sqrt(sumAllSquares);

	for (i = 0; i < myVectorSliceSize; i++)
	{
		myVectorSlice[i] = myVectorSlice[i] / globalMag;
	}

	//and then call GATHER to sew back together the individual slices of the vector back into the big vector.
	//as long as our call matches the Scatter above, we can be confident things are zipped back in the correct order
	//but BEWARE!  Only node 0 holds the proper bigVector!
	// in fact, all other nodes haven't even allocated space for it!
	// (i believe non-root nodes could replace bigVector with NULL to no ill effect,
	// but having matching function calls across all processes is good SPMD)

	MPI_Gather(myVectorSlice, myVectorSliceSize, MPI_DOUBLE, bigVector, myVectorSliceSize, MPI_DOUBLE, ROOT_NODE, MPI_COMM_WORLD);

	if (my_rank == 0)
	{
		printf("normalized vector:\n");
		for (i = 0; i < BigVectorSize; i++)
		{
			printf("%f ", bigVector[i]);
		}
		printf("\n");
	}

	MPI_Finalize();
	//finito!
	//please no MPI stuff after this line
	return 0;
}
