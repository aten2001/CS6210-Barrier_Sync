#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

int main (int argc, char * argv[]) {

  double start, end, minStart = 999999999999, maxEnd = 0;
  double *allStart, *allEnd;

  if(argc!=3)
  {
    printf("Enter the number of threads and barriers.\n"); 
    exit(0);
  }  
  
  MPI_Init(&argc, &argv);

  MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
  start = MPI_Wtime();

  int size, rank, procs, rounds, barriers;
  int i, j;
  int token = 0;

  procs = atoi(argv[1]);
  barriers = atoi(argv[2]);
  rounds = ceil(log(procs)/log(2));

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  for (i = 0; i < barriers; i++) {
    //printf("Process %d on barrier %d\n", rank, i);

    //Synchronization rounds
    for (j = 0; j < rounds; j++) {

      //Positive  numbered processes wait for odd numbered processes
      if (rank % (int)pow(2, j+1) == 0) {

        if ((rank + (int)pow(2, j)) >= procs)
	  continue;

        while (!token) {
          MPI_Recv(&token, 1, MPI_INT, (rank + (int)pow(2, j)) % procs, (int)pow(2, j), MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        token = 0;
      }

      else if (rank % (int)pow(2, j) == 0) {
   	token = 1;

  	MPI_Send(&token, 1, MPI_INT, (rank - (int)pow(2, j)) % procs, (int)pow(2, j), MPI_COMM_WORLD);
      }
    }

    //Wake up rounds
    for (j = rounds-1; j >= 0; j--) {

      //Positive  numbered processes wait for odd numbered processes
      if (rank % (int)pow(2, j+1) == 0) {

        token = 0;

  	MPI_Send(&token, 1, MPI_INT, (rank + (int)pow(2, j)) % procs, (int)pow(2, j), MPI_COMM_WORLD);
      }

      else if (rank % (int)pow(2, j) == 0){

        if ((rank - (int)pow(2, j)) < 0)
	  continue;

   	while (token) {
          MPI_Recv(&token, 1, MPI_INT, (rank - (int)pow(2, j)) % procs, (int)pow(2, j), MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
      }
    }

   // if (rank == 0)
     // printf("Synchronized on barrier %d\n", i);
  }

  MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
  end = MPI_Wtime();

  allStart = (double *)malloc(size*sizeof(double));
  allEnd = (double *)malloc(size*sizeof(double));

  MPI_Gather(&start, 1, MPI_DOUBLE, allStart, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Gather(&end, 1, MPI_DOUBLE, allEnd, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  MPI_Finalize();

  if(rank == 0) {
    for(i = 0; i < size; i++) {
      if (*(allStart + i) < minStart)
        minStart = *(allStart + i);
      if (*(allEnd + i) > maxEnd)
        maxEnd = *(allEnd + i);      
    }
    printf("%f\n", maxEnd - minStart);
  }

  return 0;
}
