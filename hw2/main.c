#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "mpi.h"
#include <unistd.h>

void main(int argc, char **argv)
{
  // Initialize MPI
  int rank, nodeCount;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nodeCount);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  printf("hi from rank %d\n", rank);
  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Finalize();
}