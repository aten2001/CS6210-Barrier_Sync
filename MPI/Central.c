#include <stdio.h>
#include <mpi.h>
#include <sys/time.h>
void barrier(int proc,int num_processes)
{
	int my_id, my_dst, my_src;
  	int tag = 1;
  	int my_msg[2];
  	MPI_Status mpi_result;
  	my_msg[0] = 0;
  	my_msg[1] = 1;
	if(proc==0)
	{
   		int i;
    		for(i=0;i<num_processes-1;i++)
    		{
    			MPI_Recv(&my_msg, 2, MPI_INT,i+1, tag, MPI_COMM_WORLD, &mpi_result);
    		}
    		for(i=0;i<num_processes-1;i++)
    		{
     			MPI_Send(&my_msg, 2, MPI_INT, i+1, tag, MPI_COMM_WORLD);
    		}
	}
	else
	{
		MPI_Send(&my_msg, 2, MPI_INT,0, tag, MPI_COMM_WORLD);
  		MPI_Recv(&my_msg, 2, MPI_INT, 0, tag, MPI_COMM_WORLD, &mpi_result);
 	}
}
int main(int argc, char **argv)
{
	struct timeval start, end,mid;
  	int my_id, num_processes,i;
    	MPI_Init(&argc, &argv);
      	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
      	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
      	gettimeofday(&start, NULL);
      	for(i=0;i<10;i++)
      	{
      		printf("Barrier start: Process %d with i %d\n",my_id,i);
      		barrier(my_id,num_processes);
      		printf("Barrier end: Process %d with i %d\n",my_id,i);
      	}
      	gettimeofday(&end, NULL);
      	printf("time taken%ld\n", ((end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec)));
      	MPI_Finalize();
      	return 0;
}

