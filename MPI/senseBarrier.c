#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
	struct timeval start, end;
	MPI_Init(NULL, NULL);
	printf("The number of Barriers in this Sense reversal barrier is 10.");
	int i, max, rank, size, lock = 1;
  	int sense, count;
  	sense = 1;
  	//*count = atoi(argv[1]);
  	count = 10;
  	max = count;
	//  MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  	MPI_Comm_size(MPI_COMM_WORLD, &size);  
	//Share the current value of count and sense with all processors
	/*  if (rank == 0)
	{
    	MPI_Bcast(&count, 1, MPI_INT, rank, MPI_COMM_WORLD);
    	MPI_Bcast(&sense, 1, MPI_INT, rank, MPI_COMM_WORLD);
  	}
	*/
  	//Start the barrier count
	gettimeofday(&start, NULL);
  	for (i = 0; i < 10; i++)
	{
		printf("Thread %d is waiting at the Barrier %d\n", rank,i);
  		lock = 0;
  		//Decrement count and broadcast the value
  		MPI_Bcast(&count, 1, MPI_INT, 0, MPI_COMM_WORLD);
  		count -= 1;
  		MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    		printf("%d\n", count);
		// If count = 0 re-initialize count to start next barrier
		if (count == 0)
		{
      			count = max;
      			sense = (sense + 1) % 2;
      			MPI_Bcast(&sense, 1, MPI_INT, 0, MPI_COMM_WORLD);
    		}
    		else
		{
      			int local;
      			local = (sense + 1) % 2;
      			while (sense != local);
    		}
		printf("Thread %d left Barrier %d\n", rank,i);
  	}
      	gettimeofday(&end, NULL);
      	printf("Total Time:	%ld\n", ((end.tv_sec * 1000000 + end.tv_usec)-(start.tv_sec * 1000000 + start.tv_usec)));
	MPI_Finalize();
	return 0;
}        
