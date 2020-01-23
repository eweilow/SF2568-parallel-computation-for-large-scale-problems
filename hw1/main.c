#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

main(int argc, char **argv)
{
  int rank, size, tag, rc, i;
  MPI_Status status;
  char message[20];
  rc = MPI_Init(&argc, &argv);
  rc = MPI_Comm_size(MPI_COMM_WORLD, &size);
  rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  tag = 100;

  int elements = size;
  int elements_per_proc = size / size;
  float sub_avg = 0.5;

  // Gather all partial averages down to the root process
  float *sub_avgs = NULL;
  if (rank == 0)
  {
    sub_avgs = malloc(sizeof(float) * size);
  }
  MPI_Gather(&sub_avg, 1, MPI_FLOAT, sub_avgs, 1, MPI_FLOAT, 0,
             MPI_COMM_WORLD);

  // Compute the total average of all numbers.
  if (rank == 0)
  {
    for (int i = 0; i < size; i++)
    {
      printf("node %d reports %.4f\n", i, sub_avgs[i]);
    }
  }

  rc = MPI_Finalize();
}