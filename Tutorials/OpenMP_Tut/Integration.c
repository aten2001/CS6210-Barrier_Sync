#include<stdio.h>
#define NUM_THREADS 10
static long num_steps =NUM_THREADS;
double step;
int main()
{
	int i; double x,pi,sum=0.0;
	step = 1.0/(double) num_steps;
	#pragma omp parallel num_threads(10)
	{
		int ID=omp_get_thread_num();
		x=(ID+0.5)*step;
		#pragma omp atomic
		sum = sum + 4.0/(1.0+ x*x);
		
	}
	pi = step*sum;
	printf("%f", pi);
}
