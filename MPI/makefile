EXECS=senseBarrier
MPICC?=mpicc

all: ${EXECS}

senseBarrier: senseBarrier.c
	${MPICC} -o senseBarrier senseBarrier.c

clean:
	rm -f ${EXECS}
