#include <mpi.h>
#include <stdio.h>
//to compile: mpicc -std=c99 -Wall MPI_blank_template.c -o MPI_blank
//to run (simple): mpirun ./MPI_blank

int main(int argc, char *argv[]){


	int        comm_sz;               /* Number of processes    */
	int        my_rank;               /* My process rank        */
	//Don't put any MPI stuff before this line

	MPI_Init(&argc, &argv);  //sets up the MPI. Always this line!
	int i;

	int valueWeWantToBroadcast;

	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	// do some stuff

//How do we get values to other processes?
if (my_rank == 0)
{
	//only Node 0 knows the secret value!
	valueWeWantToBroadcast = 42;
}

//again, ALL nodes call with the exact same line
MPI_Bcast(&valueWeWantToBroadcast,1,MPI_INT,0,MPI_COMM_WORLD);

printf("process %d has value %d\n",my_rank,valueWeWantToBroadcast);
MPI_Finalize();
	//please no MPI stuff after this line
return 0;
}
