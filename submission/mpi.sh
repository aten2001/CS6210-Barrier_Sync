#PBS -q cs6210
#PBS -l nodes=2:sixcore
#PBS -l walltime=00:02:00
#PBS -N tournament
OMPI_MCA_mpi_yield_when_idle=0
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 12 /nethome/hchawla7/CS6210-Barrier_Sync/MPI/Tournament 12 10
