set -e

export OMPI_MCA_btl_vader_single_copy_mechanism=none
mpicc -O3 -lm main.c -o main

# valgrind --suppressions=./.valgrindignore --leak-check=full ./main 1 1

mpirun -np 1 main 1 1
#mpirun -np 2 --oversubscribe main 1 2
mpirun -np 4 main 2 2
#mpirun -np 6 --oversubscribe main 2 3
#mpirun -np 8 --oversubscribe main 2 4