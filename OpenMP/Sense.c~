#include <omp.h>
#include <stdio.h>
#include <sys/time.h>
int max;
omp_lock_t countlock;
void sense_barrier(int * count,int * sense,int* local)
{
	(*local)= !(*local);
	omp_set_lock(&countlock);
	*count-=1;
	omp_unset_lock(&countlock);
	if(*count==0)
	{
		*count=max;
		*sense=*local;
		printf("Threads crossing the Barrier now!\n\n");
	}
	else
	{
		while(*sense!=*local){}
	}
}

int main(int argc, char **argv)
{
	if(argc!=2)
  	{
  		printf("Enter the number of threads.\n"); 
        	exit(0);
  	}
	printf("The number of Barriers in this Sense reversal barrier is 10.");
	struct timeval start, end;
	int count, sense=1;
	count=atoi(argv[1]);
	max=count;
	omp_set_num_threads(count);
	gettimeofday(&start, NULL);
	#pragma omp parallel shared(count,sense)
  	{
		int  thread_num = omp_get_thread_num();
	    	printf("Thread %d: Ready\n", thread_num);
	    	int i,local=1;
	   	for(i=0;i<10;i++)
	   	{
	  		printf("Thread %d is waiting at Barrier %d\n",thread_num,i);
	   		sense_barrier(&count,&sense,&local);
	  		printf("Thread %d left Barrier %d\n",thread_num,i);
   		}
	}
	gettimeofday(&end, NULL);
	printf("%Total Time:    %ld\n", ((end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec)));
	return 0;
}

