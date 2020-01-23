#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "mpi.h"
#include <unistd.h>

#define N 7

int ceilDivide(int top, int divisor)
{
  return top / divisor + (top % divisor != 0);
}

main(int argc, char **argv)
{
  int rank, size, tag, rc, i;
  MPI_Status status;
  char message[20];
  rc = MPI_Init(&argc, &argv);
  rc = MPI_Comm_size(MPI_COMM_WORLD, &size);
  rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  tag = 100;

  int columns_per_proc = ceilDivide(N, size);
  int real_size = columns_per_proc * size;

  int elements_per_proc = columns_per_proc * real_size;

  int *global_columns = NULL;
  if (rank == 0)
  {
    printf("columns_per_proc: %d\n", columns_per_proc);
    printf("elements_per_proc: %d\n", elements_per_proc);
    printf("real_size: %d\n", real_size);
    printf("real_size*real_size: %d\n", real_size*real_size);
    printf("elements_per_proc*size: %d\n", elements_per_proc*size);
    global_columns = malloc(sizeof(int) * real_size);
    for (int i = 0; i < real_size; i++)
    {
      global_columns[i] = i;
    }
  }

  int *node_columns = malloc(sizeof(int) * columns_per_proc);
  MPI_Scatter(
      global_columns,
      columns_per_proc,
      MPI_INT,
      node_columns,
      columns_per_proc,
      MPI_INT,
      0,
      MPI_COMM_WORLD);

  float *node_data = malloc(sizeof(float) * elements_per_proc);
  for (int i = 0; i < columns_per_proc; i++)
  {
    int x = node_columns[i];
    for(int y = 0; y < real_size; y++) {
      int position = i * real_size + y;

      float x_ = (float)x;
      float y_ = (float)y;
      float r = sqrt(x_*x_ + y_*y_);
      node_data[position] = cos(r / (float)real_size * 3.141592 * 2.0);
    }
  }

  float* final_data = NULL;
  if (rank == 0)
  {
    final_data = malloc(sizeof(float) * elements_per_proc * size);
  }
  MPI_Gather(node_data, elements_per_proc, MPI_FLOAT, final_data, elements_per_proc, MPI_FLOAT, 0,
             MPI_COMM_WORLD);
  // Compute the total average of all numbers.
  if (rank == 0)
  {
    for (int y = 0; y < real_size; y++)
    {
      for (int x = 0; x < real_size; x++)
      {
        printf("%8.2f", final_data[x*real_size + y]);
      }
      printf("\n");
    }
  }

  /*

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
  }*/

  rc = MPI_Finalize();
}