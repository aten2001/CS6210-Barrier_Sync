OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall 
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS)

MPICC = mpicc
MPICH = /usr/lib64/openmpi/bin/mpicc

CFLAGS = -I$(MPICH)

merge: merge.c
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) $(OMPLIBS) $(OMPFLAGS)

clean:
	rm -f *.o merge

