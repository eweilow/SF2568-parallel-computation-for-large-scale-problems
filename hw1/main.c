#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "mpi.h"
#include <unistd.h>

#define N 2000

#define MAX_STEPS 1000

#define INT int64_t
#define FLOAT long double

INT ceilDivide(INT top, INT divisor)
{
  return top / divisor + (top % divisor != 0);
}

void divideData(INT requestSize, INT nodeCount, INT *columnsPerNode, INT *actualSize) {
  *columnsPerNode = ceilDivide(requestSize, nodeCount);
  *actualSize = *columnsPerNode * nodeCount;
}

struct Imaginary {
  FLOAT a;
  FLOAT b;
};

void multiplyImaginary(
  struct Imaginary *left,
  struct Imaginary *right,
  struct Imaginary *output
) {
  FLOAT a_ = left->a * right->a - left->b*right->b;
  FLOAT b_ = left->b * right->a + left->a*right->b;
  output->a = a_;
  output->b = b_;
}

void addImaginary(
  struct Imaginary *left,
  struct Imaginary *right,
  struct Imaginary *output
) {
  FLOAT a_ = left->a + right->a;
  FLOAT b_ = left->b + right->b;
  output->a = a_;
  output->b = b_;
}

FLOAT sqLengthImaginary(
  struct Imaginary *im
) {
  return im->a*im->a + im->b*im->b;
}

INT calculatePixel(
  FLOAT x,
  FLOAT y
) {
  const FLOAT b = 2.0;
  const FLOAT b2 = b * b;

  struct Imaginary m = {
    x, y
  };

  struct Imaginary z = {
    0.0, 0.0
  };

  INT i = 0;
  for(; i < MAX_STEPS; i++) {
    // z_(i+1) = z_(i)^2 + (x, y)
    multiplyImaginary(&z, &z, &z); // z = z^2
    addImaginary(&z, &m, &z); // z = z + m

    // |z_n| < b ∀n
    if(sqLengthImaginary(&z) >= b2) {
      break;
    }
  }

  return i;
}

#define TIME() ((double) (clock())) / CLOCKS_PER_SEC;

void main(int argc, char **argv)
{
  int rank, nodeCount;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nodeCount);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  INT columnsPerNode, actualSize;
  divideData(N, nodeCount, &columnsPerNode, &actualSize);

  INT pixelsPerNode = columnsPerNode * actualSize;

  MPI_Barrier(MPI_COMM_WORLD);
  double startTime = TIME();

  INT *global_columns = NULL;
  if (rank == 0)
  {
    printf("columnsPerNode: %d\n", columnsPerNode);
    printf("pixelsPerNode: %d\n", pixelsPerNode);
    printf("actualSize: %d\n", actualSize);
    printf("actualSize*actualSize: %d\n", actualSize*actualSize);
    printf("pixelsPerNode*nodeCount: %d\n", pixelsPerNode*nodeCount);
    global_columns = malloc(sizeof(INT) * actualSize);
    for (INT i = 0; i < actualSize; i++)
    {
      global_columns[i] = i;
    }
  }

  INT *node_columns = malloc(sizeof(INT) * columnsPerNode);
  MPI_Scatter(global_columns, columnsPerNode, MPI_INT64_T, node_columns, columnsPerNode, MPI_INT64_T, 0, MPI_COMM_WORLD);

  /*
  const FLOAT cx = -0.9973;
  const FLOAT cy = -0.2875;
  const FLOAT scale = 15000;
  */

  const FLOAT cx = -0.9973;
  const FLOAT cy = -0.2875;
  const FLOAT scale = 15000;

  INT *node_data = malloc(sizeof(INT) * pixelsPerNode);
  for (INT i = 0; i < columnsPerNode; i++)
  {
    INT x = node_columns[i];
    for(INT y = 0; y < actualSize; y++) {
      INT position = i * actualSize + y;

      FLOAT x_ = ((FLOAT)x / ((FLOAT) actualSize - 1.0) - 0.5) * 2.0 / scale + cx;
      FLOAT y_ = ((FLOAT)y / ((FLOAT) actualSize - 1.0) - 0.5) * 2.0 / scale + cy;
      node_data[position] = calculatePixel(x_, y_);
      // FLOAT r = sqrt(x_*x_ + y_*y_);
      // node_data[position] = (INT) ceil(1000.0 * cos(r / (sqrt(2.0)) * 3.141592 * 2.0));
    }
  }

  INT* final_data = NULL;
  if (rank == 0)
  {
    final_data = malloc(sizeof(INT) * pixelsPerNode * nodeCount);
  }

  MPI_Gather(node_data, pixelsPerNode, MPI_INT64_T, final_data, pixelsPerNode, MPI_INT64_T, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  double endTime = TIME();
  if(rank == 0) {
    printf("%d nodes, %.4lf ms\n", nodeCount, 1000.0*(endTime - startTime));
  }

  if (rank == 0)
  {
/*
    for (INT  y = 0; y < actualSize; y++)
    {
      for (INT  x = 0; x < actualSize; x++)
      {
        printf("%8ld", final_data[x*actualSize + y]);
      }
      printf("\n");
    }
*/  
    const double minX = cx - 1.0/scale;
    const double maxX = cx + 1.0/scale;
    const double minY = cy - 1.0/scale;
    const double maxY = cy + 1.0/scale;

    FILE* file = fopen("/etc/data/data.bin", "w");
    fwrite(&minX, sizeof(double), 1, file);
    fwrite(&maxX, sizeof(double), 1, file);
    fwrite(&minY, sizeof(double), 1, file);
    fwrite(&maxY, sizeof(double), 1, file);
    fwrite(&actualSize, sizeof(INT), 1, file);
    fwrite(final_data, sizeof(INT), actualSize*actualSize, file);
    fclose(file);
  }


  MPI_Finalize();
}