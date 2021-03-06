/*
    From the MCS Paper: A scalable, distributed tournament barrier with only local spinning
    type round_t = record
        role : (winner, loser, bye, champion, dropout)
	opponent : ^Boolean
	flag : Boolean
    shared rounds : array [0..P-1][0..LogP] of round_t
        // row vpid of rounds is allocated in shared memory
	// locally accessible to processor vpid
    processor private sense : Boolean := true
    processor private vpid : integer // a unique virtual processor index
    //initially
    //    rounds[i][k].flag = false for all i,k
    //rounds[i][k].role = 
    //    winner if k > 0, i mod 2^k = 0, i + 2^(k-1) < P , and 2^k < P
    //    bye if k > 0, i mode 2^k = 0, and i + 2^(k-1) >= P
    //    loser if k > 0 and i mode 2^k = 2^(k-1)
    //    champion if k > 0, i = 0, and 2^k >= P
    //    dropout if k = 0
    //    unused otherwise; value immaterial
    //rounds[i][k].opponent points to 
    //    round[i-2^(k-1)][k].flag if rounds[i][k].role = loser
    //    round[i+2^(k-1)][k].flag if rounds[i][k].role = winner or champion
    //    unused otherwise; value immaterial
    procedure tournament_barrier
        round : integer := 1
	loop   //arrival
	    case rounds[vpid][round].role of
	        loser:
	            rounds[vpid][round].opponent^ :=  sense
		    repeat until rounds[vpid][round].flag = sense
		    exit loop
   	        winner:
	            repeat until rounds[vpid][round].flag = sense
		bye:  //do nothing
		champion:
	            repeat until rounds[vpid][round].flag = sense
		    rounds[vpid][round].opponent^ := sense
		    exit loop
		dropout: // impossible
	    round := round + 1
	loop  // wakeup
	    round := round - 1
	    case rounds[vpid][round].role of
	        loser: // impossible
		winner:
		    rounds[vpid[round].opponent^ := sense
		bye: // do nothing
		champion: // impossible
		dropout:
		    exit loop
	sense := not sense
*/
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include<stdlib.h>
#include <stdbool.h>
#include<sys/time.h>
#define winner 0
#define loser 1
#define bye 2
#define champion 3
#define dropout 4

struct round_t
{
	int role;
	int vpid;
	int tb_round;
	bool *opponent;
	bool flag;
};
typedef struct round_t round_t;
round_t array[100][100];

void tournament_barrier( int vpid, bool *sense,int num_rounds)
{
	int round = 0;
	while(1)
	{

		if(array[vpid][round].role == loser)
		{
			*( array[vpid][round] ).opponent = *sense;
			while( array[vpid][round].flag != *sense );
			break;
		}

		if( array[vpid][round].role == winner )
		{
			while( array[vpid][round].flag != *sense );
		}
		if( array[vpid][round].role == champion )
		{
			while( array[vpid][round].flag != *sense );
			*( array[vpid][round] ).opponent = *sense;
			break;
		}

		if(round < num_rounds)
		{
			round = round + 1;
		}
	}
	while(1) 
	{
		if( round > 0 )
		{
			round = round - 1;
		}

		if( array[vpid][round].role == winner )
		{
			*( array[vpid][round] ).opponent = *sense;
		}
		if( array[vpid][round].role == dropout )
		{
			break;
		}
	}

	*sense = !*sense;
}

int main(int argc, char **argv)
{
	bool x = false;

	if(argc!=3)
  	{
  		printf("Enter the number of threads and barriers.\n"); 
        	exit(0);
  	}
	struct timeval start, end;
	int i,k;
	int thread_count = atoi(argv[1]);
	int barrier_count = atoi(argv[2]);
	int num_rounds = ceil( log(thread_count)/log(2) );
	for(i=0;i<thread_count;i++)
	{
		for(k=0;k<=num_rounds;k++)
		{
			array[i][k].flag = false;
			array[i][k].role = -1;
			array[i][k].opponent = &x;
		}
	}
	int second_check, first_check=0;

	for(i=0 ; i<thread_count; i++)
	{
		for(k=0;k<=num_rounds;k++)
		{
			second_check = ceil( pow(2,k) );
			first_check = ceil( pow(2,k-1) );

			if((k > 0) && (i%second_check==0) && ((i + (first_check))< thread_count) && (second_check < thread_count))
			{
				array[i][k].role = winner;
			}

			if((k > 0) && (i%second_check == 0) && ((i + first_check)) >= thread_count)
			{
				array[i][k].role = bye;
			}

			if((k > 0) && ((i%second_check == first_check)))
			{
				array[i][k].role = loser;
			}

			if((k > 0) && (i==0) && (second_check >= thread_count))
			{
				array[i][k].role = champion;
			}

			if(k==0)
			{
				array[i][k].role = dropout;
			}
			if(array[i][k].role == loser)
			{
				array[i][k].opponent = &array[i-first_check][k].flag;
			}
			if(array[i][k].role == winner || array[i][k].role == champion)
			{
				array[i][k].opponent = &array[i+first_check][k].flag;
			}
		}
	}
	gettimeofday(&start, NULL);
	#pragma omp parallel num_threads(thread_count) shared(array)
	{
		int vpid=0;
		bool *sense;
		bool temp = true;
		#pragma omp critical
		{
			vpid = omp_get_thread_num();
			sense = &temp;
		}
		int num_threads = omp_get_num_threads();
	 	int i,j;
		int thread_num = omp_get_thread_num();
		printf( "Thread Number: %d Ready.\n", vpid);
		for( i = 0; i < barrier_count; i++ )
		{
			printf("Thread %d is waiting at Barrier %d.\n",vpid,i+1);
			tournament_barrier(vpid,sense,num_rounds);
			printf("Thread %d left Barrier %d.\n",vpid,i+1);
	    	}
	}
	gettimeofday(&end, NULL);
	printf("Total Time:    %ld\n", ((end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec)));
	return 0;
}
