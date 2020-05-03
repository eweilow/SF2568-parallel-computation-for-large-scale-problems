#!/bin/bash
#SBATCH -A <account>
#SBATCH -n 36
#SBATCH --time=00:05:00

echo "16c"
srun -n 16 main 4 4 &
srun -n 16 main 4 4 &
wait # total 32 concurrent

echo "16c"
srun -n 16 main 4 4 &
srun -n 16 main 4 4 &
wait # total 32 concurrent

echo "16c"
srun -n 16 main 4 4 &
srun -n 16 main 4 4 &
wait # total 32 concurrent

echo "12c"
srun -n 12 main 4 3 &
srun -n 12 main 4 3 &
srun -n 12 main 4 3 &
wait # total 36 concurrent

echo "12c"
srun -n 12 main 3 4 &
srun -n 12 main 3 4 &
srun -n 12 main 3 4 &
wait # total 36 concurrent

echo "8c"
srun -n 8 main 4 2 &
srun -n 8 main 4 2 &
srun -n 8 main 4 2 &
srun -n 8 main 4 2 &
wait # total 32 concurrent

echo "8c"
srun -n 8 main 2 4 &
srun -n 8 main 2 4 &
srun -n 8 main 2 4 &
srun -n 8 main 2 4 &
wait # total 32 concurrent

echo "4c"
srun -n 4 main 4 1 &
srun -n 4 main 4 1 &
srun -n 4 main 4 1 &
srun -n 4 main 1 4 &
srun -n 4 main 1 4 &
srun -n 4 main 1 4 &
srun -n 4 main 2 2 &
srun -n 4 main 2 2 &
srun -n 4 main 2 2 &
wait  # total 36 concurrent

echo "2c"
srun -n 2 main 2 1 &
srun -n 2 main 2 1 &
srun -n 2 main 2 1 &
srun -n 2 main 2 1 &
srun -n 2 main 1 2 &
srun -n 2 main 1 2 &
srun -n 2 main 1 2 &
srun -n 2 main 1 2 &
wait # total 12 concurrent

echo "1c"
srun -n 1 main 1 1 &
srun -n 1 main 1 1 &
srun -n 1 main 1 1 &
srun -n 1 main 1 1 &
srun -n 1 main 1 1 &
srun -n 1 main 1 1 &
srun -n 1 main 1 1 &
srun -n 1 main 1 1 &
wait # total 6 concurrent