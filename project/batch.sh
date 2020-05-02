#!/bin/bash
#SBATCH -A <account>
#SBATCH -n 36
#SBATCH --time=00:30:00

export OMPI_MCA_btl_vader_single_copy_mechanism=none

echo "16c"
mpirun -np 16 --oversubscribe main 4 4
mpirun -np 16 --oversubscribe main 4 4
wait # total 32 concurrent

echo "16c"
mpirun -np 16 --oversubscribe main 4 4
mpirun -np 16 --oversubscribe main 4 4
wait # total 32 concurrent

echo "16c"
mpirun -np 16 --oversubscribe main 4 4
mpirun -np 16 --oversubscribe main 4 4
wait # total 32 concurrent

echo "12c"
mpirun -np 12 --oversubscribe main 4 3
mpirun -np 12 --oversubscribe main 4 3
mpirun -np 12 --oversubscribe main 4 3
wait # total 36 concurrent

echo "12c"
mpirun -np 12 --oversubscribe main 3 4
mpirun -np 12 --oversubscribe main 3 4
mpirun -np 12 --oversubscribe main 3 4
wait # total 36 concurrent

echo "8c"
mpirun -np 8 --oversubscribe main 4 2
mpirun -np 8 --oversubscribe main 4 2
mpirun -np 8 --oversubscribe main 4 2
mpirun -np 8 --oversubscribe main 4 2
wait # total 32 concurrent

echo "8c"
mpirun -np 8 --oversubscribe main 2 4
mpirun -np 8 --oversubscribe main 2 4
mpirun -np 8 --oversubscribe main 2 4
mpirun -np 8 --oversubscribe main 2 4
wait # total 32 concurrent

echo "4c"
mpirun -np 4 --oversubscribe main 4 1
mpirun -np 4 --oversubscribe main 4 1
mpirun -np 4 --oversubscribe main 4 1
mpirun -np 4 --oversubscribe main 1 4
mpirun -np 4 --oversubscribe main 1 4
mpirun -np 4 --oversubscribe main 1 4
mpirun -np 4 --oversubscribe main 2 2
mpirun -np 4 --oversubscribe main 2 2
mpirun -np 4 --oversubscribe main 2 2
wait  # total 36 concurrent

echo "2c"
mpirun -np 2 --oversubscribe main 2 1
mpirun -np 2 --oversubscribe main 2 1
mpirun -np 2 --oversubscribe main 2 1
mpirun -np 2 --oversubscribe main 2 1
mpirun -np 2 --oversubscribe main 1 2
mpirun -np 2 --oversubscribe main 1 2
mpirun -np 2 --oversubscribe main 1 2
mpirun -np 2 --oversubscribe main 1 2
wait # total 12 concurrent

echo "1c"
mpirun -np 1 --oversubscribe main 1 1
mpirun -np 1 --oversubscribe main 1 1
mpirun -np 1 --oversubscribe main 1 1
mpirun -np 1 --oversubscribe main 1 1
mpirun -np 1 --oversubscribe main 1 1
mpirun -np 1 --oversubscribe main 1 1
mpirun -np 1 --oversubscribe main 1 1
mpirun -np 1 --oversubscribe main 1 1
wait # total 6 concurrent