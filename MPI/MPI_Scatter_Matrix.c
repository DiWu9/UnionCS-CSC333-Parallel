#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
//to compile: mpicc -std=c99 -Wall MPI_blank_template.c -o MPI_blank
//to run (simple): mpirun ./MPI_blank

int main(int argc, char *argv[]){


	int        comm_sz;               /* Number of processes    */
	int        my_rank;               /* My process rank        */
	//Don't put any MPI stuff before this line


	int *bigArray;
	MPI_Init(&argc, &argv);  //sets up the MPI. Always this line!
	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	// do some stuff
	int destinations[9][9] = {
		{0, 0, 0, 1, 1, 1, 2, 2, 2},
		{0, 0, 0, 1, 1, 1, 2, 2, 2},
		{0, 0, 0, 1, 1, 1, 2, 2, 2},
		{3, 3, 3, 4, 4, 4, 5, 5, 5},
		{3, 3, 3, 4, 4, 4, 5, 5, 5},
		{3, 3, 3, 4, 4, 4, 5, 5, 5},
		{6, 6, 6, 7, 7, 7, 8, 8, 8},
		{6, 6, 6, 7, 7, 7, 8, 8, 8},
		{6, 6, 6, 7, 7, 7, 8, 8, 8}
	};

  int numRows = 9;
	int numCols = 9;
	int myNumRows = numRows/comm_sz;
	int matrixSize = numRows*numCols;

	// this is for the simple scatter
	int *myRows = malloc(myNumRows*numCols*sizeof(int));

  //this is for checkerboard distribution
	int submatrixsize = numRows*numCols/comm_sz;
	int *mySubMatrix = malloc(submatrixsize*sizeof(int));

	int* theOriginalArray;

//How do we get values to other processes?
if (my_rank == 0)
{
	//
	int originArray[9][9] = {
		{0,   0,  0, 10, 10, 10, 20, 20, 20},
		{0,   0,  0, 10, 10, 10, 20, 20, 20},
		{0,   0,  0, 10, 10, 10, 20, 20, 20},
		{30, 30, 30, 40, 40, 40, 50, 50, 50},
		{30, 30, 30, 40, 40, 40, 50, 50, 50},
		{30, 30, 30, 40, 40, 40, 50, 50, 50},
		{60, 60, 60, 70, 70, 70, 80, 80, 80},
		{60, 60, 60, 70, 70, 70, 80, 80, 80},
		{60, 60, 60, 70, 70, 70, 80, 80, 80}
	};

// we need all processes to at least *think* there is a variable called theOriginalArray
	theOriginalArray = &originArray[0][0];
	//only Node 0 knows the secret value!
}

	MPI_Scatter(theOriginalArray,myNumRows*numCols,MPI_INT,myRows,myNumRows*numCols,MPI_INT,0,MPI_COMM_WORLD);

int i;

// for (i = 0; i < myNumRows*numCols; i++){
// 		printf("%d ",myRows[i]);
// 		}
// 			printf("\n");


int destinationCounter;

//cycle through the destinations, and send each row to the right place

int slicelength = 3;

if (my_rank == 0)
{
		int sentToSelf = 0;
	for (i= 0; i < matrixSize; i++)
	{
		int destination = *(&destinations[0][0]+i);
		int data = theOriginalArray[i];
		if (destination == 0){
				mySubMatrix[sentToSelf] = data;
				printf("sending %d to myself at index %d, and got %d\n",data,sentToSelf,mySubMatrix[sentToSelf]);
				sentToSelf++;
		}
		else{
			MPI_Send(&data,1,MPI_INT,destination,0,MPI_COMM_WORLD);
		}
	}
}
	//every other node waits for the 9 messages the root node will send it, in order
else{
		int received = 0;
		while (received < submatrixsize){
			int indata = 0;
			MPI_Recv(&indata,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			mySubMatrix[received] = indata;
			received++;
		}

	}

if (my_rank == 0)
{
		for (i = 0; i < submatrixsize; i++)
		{
					printf("%d ",mySubMatrix[i]);
		}
					printf("\n");
}



MPI_Finalize();
	//please no MPI stuff after this line
return 0;
}
