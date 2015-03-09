EXECS=tournamentBarrier disseminationBarrier
MPICC?=mpicc

all: ${EXECS}

tournamentBarrier: tournamentBarrier.c
	${MPICC} -o tournamentBarrier tournamentBarrier.c -lm

disseminationBarrier: disseminationBarrier.c
	${MPICC} -o disseminationBarrier disseminationBarrier.c -lm

clean:
	rm -f ${EXECS}
