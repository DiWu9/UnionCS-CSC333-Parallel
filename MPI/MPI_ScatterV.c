#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//to compile: mpicc -std=c99 -Wall MPI_blank_template.c -o MPI_blank
//to run (simple): mpirun ./MPI_blank

int main(int argc, char *argv[]){


	int        comm_sz;               /* Number of processes    */
	int        my_rank;               /* My process rank        */
	//Don't put any MPI stuff before this line

double *bigVector;
	double  *myVectorSlice;
	int BigVectorSize = 200;

	int *bigArray;
	int localArray[5];

	int i;
	int valueWeWantToBroadcast;

	MPI_Init(&argc, &argv);  //sets up the MPI. Always this line!
	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	// do some stuff


//make an array corresponding to the number of tasks running
	int * sendcounts = malloc(sizeof(int)*comm_sz);
//make an array corresponding to the number of tasks running
	int * displacements = malloc(sizeof(int)*comm_sz);

	int LocalSliceSize = (int)(floor(BigVectorSize/comm_sz));

	int Remainder = BigVectorSize%comm_sz;

if (my_rank == 0){
	printf("LocalSliceSize: %d, Remiander: %d\n",LocalSliceSize,Remainder);
	printf("All processes will receive %d values, but the first %d processors will receive an extra value\n",LocalSliceSize,Remainder);
}

//	rintf("slice %d: %d\n",my_rank,LocalSliceSize);


//a little extra pudding for me!
//	if (my_rank < Remainder){
	//			LocalSliceSize+=1;
//	}
//a little extra room for padding.
	myVectorSlice = malloc(sizeof(double)*LocalSliceSize+1);


if (my_rank == 0){
	printf("LocalSliceSize: %d, Remiander: %d\n",LocalSliceSize,Remainder);
	printf("All processes will receive %d values, but the first %d processors will receive an extra value\n",LocalSliceSize,Remainder);
}

int curdisplacement = 0;

for(i = 0; i < comm_sz; i++)
{
	//notice that in the ABOVE code we varied this.
	sendcounts[i] = LocalSliceSize;
	if (i < Remainder) {
			sendcounts[i]++;
	}
	displacements[i] = curdisplacement;
	curdisplacement += sendcounts[i]; //displacement from the START of the
	 																	//vector we are scattering.
			if(my_rank == 0){
					printf("for %d, %d slice size, %d total now\n",i,sendcounts[i],curdisplacement);
			}
}


//How do we get values to other processes?
if (my_rank == 0)
{
		bigVector = malloc(BigVectorSize*sizeof(double));
	//printf("original vector:\n");
	int i;
	for (i = 0; i < BigVectorSize; i++)
	{
		bigVector[i] = drand48();      // returns a pseudo-random integer between 0 and RAND_MAXA
																			//then cast as a double.  sloppy but effective
		printf("%f ",bigVector[i]);
	}
	printf("\n");
}

MPI_Scatterv(bigVector,sendcounts,displacements,MPI_DOUBLE,
						myVectorSlice,LocalSliceSize+1,MPI_DOUBLE,0,MPI_COMM_WORLD);


printf("%d: ",my_rank);
for (i = 0; i < LocalSliceSize; i++)
{
	printf(" %f ",myVectorSlice[i]);
}
printf("\n");

MPI_Finalize();
	//please no MPI stuff after this line
return 0;
}
