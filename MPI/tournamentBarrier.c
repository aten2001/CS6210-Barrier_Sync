#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>

int main (int argc, char * argv) {
  MPI_Init(NULL, NULL);

  int size, rank, procs = 8, rounds = 3, barriers = 10;
  int i, j;
  int token = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  for (i = 0; i < barriers; i++) {
    printf("Process %d on barrier %d\n", rank, i);

    //Synchronization rounds
    for (j = 0; j < rounds; j++) {

      //Positive  numbered processes wait for odd numbered processes
      if (rank % (int)pow(2, j+1) == 0) {
        printf("Process %d waiting on process %d in round %d\n", rank, rank + (int)pow(2, j), (int)pow(2, j));
        while (!token) {
          MPI_Recv(&token, 1, MPI_INT, rank + (int)pow(2, j), (int)pow(2, j), MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        token = 0;
      }

      else if (rank % (int)pow(2, j) == 0) {
   	token = 1;
        printf("Process %d sending to %d in round %d\n", rank, rank - (int)pow(2, j), (int)pow(2, j));
  	MPI_Send(&token, 1, MPI_INT, rank - (int)pow(2, j), (int)pow(2, j), MPI_COMM_WORLD);
      }
    }

    //Wake up rounds
    for (j = rounds-1; j >= 0; j--) {

      //Positive  numbered processes wait for odd numbered processes
      if (rank % (int)pow(2, j+1) == 0) {
        printf("Process %d waking up process %d in round %d\n", rank, rank + (int)pow(2, j), (int)pow(2, j));
        token = 0;
        printf("Process %d sending to %d in round %d\n", rank, rank + (int)pow(2, j), (int)pow(2, j));
  	MPI_Send(&token, 1, MPI_INT, rank + (int)pow(2, j), (int)pow(2, j), MPI_COMM_WORLD);
      }

      else if (rank % (int)pow(2, j) == 0){
        printf("Process %d waiting on process %d in round %d\n", rank, (rank - (int)pow(2, j)) % 8, (int)pow(2, j));
   	while (token) {
          MPI_Recv(&token, 1, MPI_INT, (rank - (int)pow(2, j)) % 8, (int)pow(2, j), MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
      }
    }
    printf("Synchronized on barrier %d\n", i);
  }
}
