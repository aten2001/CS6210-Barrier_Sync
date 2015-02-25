#include<stdio.h>
#include<omp.h>
int main()
{
	#pragma omp parallel num_threads(10)
	{
		int ID=omp_get_thread_num();
		printf("hello(%d)",ID);
		printf("world(%d)",ID);
	}
}
