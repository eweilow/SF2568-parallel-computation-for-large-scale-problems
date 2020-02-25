#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "mpi.h"
#include <unistd.h>

#define MAX_STEPS 1000

#define INT int64_t
#define FLOAT double

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

// Multiply two imaginary numbers
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

// Add two imaginary numbers
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

// Square length of imaginary number
FLOAT sqLengthImaginary(
  struct Imaginary *im
) {
  return im->a*im->a + im->b*im->b;
}

// Compute a pixel of the Mandelbrot set, returning the iteration count
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
    multiplyImaginary(&z, &z, &z); // z = z^2
    addImaginary(&z, &m, &z); // z = z + m

    // Break if magnitude is too large
    if(sqLengthImaginary(&z) >= b2) {
      break;
    }
  }

  return i;
}

// Macro that provides the current time in seconds
#define TIME() ((double) (clock())) / CLOCKS_PER_SEC;

void main(int argc, char **argv)
{
  char *endPtr;
  int N = strtol(argv[2], &endPtr, 10);

  // Compute arguments from argv
  FLOAT cx = (FLOAT) strtod(argv[3], &endPtr);
  FLOAT cy = (FLOAT) strtod(argv[4], &endPtr);
  FLOAT scale = (FLOAT) strtod(argv[5], &endPtr);

  // Initialize MPI
  int rank, nodeCount;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nodeCount);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Set up the data handling
  INT columnsPerNode, actualSize;
  divideData(N, nodeCount, &columnsPerNode, &actualSize);
  INT pixelsPerNode = columnsPerNode * actualSize;

  // Print argv on p_0
  if(rank == 0) {
    printf("Name: %s\n", argv[1]);
    printf("Size: %d\n", N);
    printf("cx: %lf\n", cx);
    printf("cy: %lf\n", cy);
    printf("scale: %lf\n", scale);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  double startTime = TIME();

  // global_columns contains the indices of all columns to be computed
  INT *global_columns = NULL;
  // global_columns is only used on p_0, so only allocate it there
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
  
  // Allocate node_columns on all nodes (which will contain the columns that this process is to compute)
  INT *node_columns = malloc(sizeof(INT) * columnsPerNode);
  // Scatter data from node_columns on p_0 into node_columns on all processes p_i
  MPI_Scatter(global_columns, columnsPerNode, MPI_INT64_T, node_columns, columnsPerNode, MPI_INT64_T, 0, MPI_COMM_WORLD);

  // Allocate node_data on all nodes, which will contain iteration counts in each pixel
  INT *node_data = malloc(sizeof(INT) * pixelsPerNode);
  for (INT i = 0; i < columnsPerNode; i++)
  {
    INT x = node_columns[i];
    for(INT y = 0; y < actualSize; y++) {
      // Position in node_data
      INT position = i * actualSize + y;

      // Compute coordinates (a, b) for this pixel (x, y)
      FLOAT a = ((FLOAT)x / ((FLOAT) actualSize - 1.0) - 0.5) * 2.0 / scale + cx;
      FLOAT b = ((FLOAT)y / ((FLOAT) actualSize - 1.0) - 0.5) * 2.0 / scale + cy;
      node_data[position] = calculatePixel(a, b);
    }
    
    const INT printSteps = 25;
    // Print progress on all nodes every 25 pixels
    if((i + 1 + printSteps) % printSteps == 0) {
      FLOAT percentage = ((FLOAT) i + 1) / ((FLOAT) columnsPerNode) * (FLOAT)100.0;
      printf("%d %.2lf %% done\n", rank, percentage);
    }
  }

  // final_data contains the count of iterations run in each pixel
  INT* final_data = NULL;
  // final_data is only used on p_0, so only allocate it there
  if (rank == 0)
  {
    final_data = malloc(sizeof(INT) * pixelsPerNode * nodeCount);
  }

  // Gather data from node_data on p_i into final_data on p_0
  MPI_Gather(node_data, pixelsPerNode, MPI_INT64_T, final_data, pixelsPerNode, MPI_INT64_T, 0, MPI_COMM_WORLD);

  // Sync all processes
  MPI_Barrier(MPI_COMM_WORLD);
  
  double endTime = TIME();
  if(rank == 0) {
    printf("%d nodes, %.4lf ms\n", nodeCount, 1000.0*(endTime - startTime));
  }

  // Dump final_data into a file on p_0
  if (rank == 0)
  {
    const double minX = cx - 1.0/scale;
    const double maxX = cx + 1.0/scale;
    const double minY = cy - 1.0/scale;
    const double maxY = cy + 1.0/scale;

    char name[100];
    sprintf(name, "/etc/data/%s.bin", argv[1]);

    FILE* file = fopen(name, "w");
    if(file == NULL) {
      printf("Error opening %s!\n", name);
      return 1;
    }

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