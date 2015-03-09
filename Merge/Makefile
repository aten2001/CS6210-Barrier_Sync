OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall 
LDLIBS = $(OMPLIBS)

MPICC = mpicc
MPICH = /usr/lib64/openmpi/bin/mpicc

CFLAGS = -I$(MPICH)

merge: merge.c
	$(MPICC) -o $@ $^ $(LDLIBS) $(OMPLIBS) $(OMPFLAGS) -DGIMP_DISABLE_DEPRECATED /lib64/libm.so.6
clean:
	rm -f *.o merge

