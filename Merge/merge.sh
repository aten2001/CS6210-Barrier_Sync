#PBS -q cs6210
#PBS -l nodes=1:fourcore
#PBS -l walltime=00:01:00
#PBS -N Merge
OMPI_MCA_mpi_yield_when_idle=0
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 4 /nethome/hchawla/CS6210-Barrier_Sync/Merge/merge 4 1 
