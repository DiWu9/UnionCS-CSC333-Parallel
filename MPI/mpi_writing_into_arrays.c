/* File:       
 *    mpi_writing_into_arrays.c
 *
 * Purpose:    
 *    A program that uses MPI to write into a string
 *
 *
 * Input:      
 *    None
 * Output:     
 *    A greeting from each process
 *
 * Algorithm:  
 *    Each process sends a message to process 0, which prints 
 *    the messages it has received, as well as its own message.
 *
 * IPP:  Section 3.1 (pp. 84 and ff.)
 */
#include <stdio.h>
#include <string.h>  /* For strlen             */
#include <mpi.h>     /* For MPI functions, etc */ 

const int MAX_STRING = 100;

int main(void) {
   char       greeting[MAX_STRING];  /* String storing message */
   int        comm_sz;               /* Number of processes    */
   int        my_rank;               /* My process rank        */\
   char       my_name[MAX_STRING];
   int        name_len = MAX_STRING;
   /* Start up MPI */
   MPI_Init(NULL, NULL); 
   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
  MPI_Get_processor_name( my_name, &name_len);
   if (my_rank != 0) { 
      /* Create message */
      sprintf(greeting, "%d",my_rank); 
      /* Send message to process 0 */
      MPI_Send(greeting, 1, MPI_CHAR, 0, 0,
            MPI_COMM_WORLD); 
   } else {  
      /* Print my message */
      sprintf(greeting,"I am process zero and this is a long string");
      for (int q = 1; q < comm_sz; q++) {
         /* Receive message from process q */
         MPI_Recv(greeting+2*q, 1, MPI_CHAR, q,
            0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         /* Print message from process q */
         printf("%s\n", greeting);
      } 
   }
   /* Shut down MPI */
   MPI_Finalize(); 
   return 0;
}  /* main */
