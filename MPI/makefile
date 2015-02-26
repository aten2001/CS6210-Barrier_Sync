EXECS=senseBarrier tournamentBarrier
MPICC?=mpicc

all: ${EXECS}

senseBarrier: senseBarrier.c
	${MPICC} -o senseBarrier senseBarrier.c

tournamentBarrier: tournamentBarrier.c
	${MPICC} -o tournamentBarrier tournamentBarrier.c -lm

clean:
	rm -f ${EXECS}
