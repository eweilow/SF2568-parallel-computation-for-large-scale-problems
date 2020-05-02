set -e

export OMPI_MCA_btl_vader_single_copy_mechanism=none
mpicc -O3 -DGEOM_CASE=1 -lm main.c -o main
mpicc -O3 -DGEOM_CASE=1 -DSAVE_DATA=1 -lm main.c -o main_save

# valgrind --suppressions=./.valgrindignore --leak-check=full ./main 1 1

# echo ""
# echo "Running on 1 CPUs"
# mpirun -np 1 main 1 1

echo ""
echo "Saving from 4 CPUs"
mpirun -np 4 main_save 2 2

echo ""
echo "Running benchmark"
mpirun -np 8 --oversubscribe main 4 2
mpirun -np 8 --oversubscribe main 4 2
mpirun -np 8 --oversubscribe main 2 4
mpirun -np 8 --oversubscribe main 2 4

mpirun -np 6 --oversubscribe main 3 2
mpirun -np 6 --oversubscribe main 3 2
mpirun -np 6 --oversubscribe main 2 3
mpirun -np 6 --oversubscribe main 2 3

mpirun -np 4 main 1 4
mpirun -np 4 main 1 4
mpirun -np 4 main 1 4
mpirun -np 4 main 1 4

mpirun -np 4 main 4 1
mpirun -np 4 main 4 1
mpirun -np 4 main 4 1
mpirun -np 4 main 4 1

mpirun -np 4 main 2 2
mpirun -np 4 main 2 2
mpirun -np 4 main 2 2
mpirun -np 4 main 2 2

mpirun -np 2 main 1 2
mpirun -np 2 main 1 2
mpirun -np 2 main 1 2
mpirun -np 2 main 1 2

mpirun -np 2 main 2 1
mpirun -np 2 main 2 1
mpirun -np 2 main 2 1
mpirun -np 2 main 2 1

mpirun -np 1 main 1 1
mpirun -np 1 main 1 1
mpirun -np 1 main 1 1
mpirun -np 1 main 1 1
# echo ""
# echo "Running on 2 CPUs"
# mpirun -np 2 main 1 2

#mpirun -np 2 --oversubscribe main 1 2
# mpirun -np 4 main 2 2
#mpirun -np 6 --oversubscribe main 2 3
#mpirun -np 8 --oversubscribe main 2 4