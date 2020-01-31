#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "mpi.h"
#include <unistd.h>

#define N 1024

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))


struct ScatterOperation {
  int nodeCount;
  int totalCount;
  int idealOperationsPerNode;
  int* sendCounts;
  int* displacements;
};

struct GatherOperation {
  int totalCount;
  int outputElementsPerInputElement;
  int* sendCounts;
  int* displacements;
};

struct ScatterOperation createScatter(int dataLength, int nodeCount) {
  struct ScatterOperation result = *(struct ScatterOperation*)malloc(sizeof(struct ScatterOperation));

  result.idealOperationsPerNode = dataLength / nodeCount;
  result.totalCount = dataLength;

  result.sendCounts = malloc(sizeof(int) * nodeCount);
  result.displacements = malloc(sizeof(int) * nodeCount);
  result.nodeCount = nodeCount;

  int remaining = dataLength;
  int offset = 0;

  for(int i = 0; i < nodeCount; i++) {
    int currentCount = min(result.idealOperationsPerNode + (i%2), max(0, remaining));
    result.sendCounts[i] = currentCount;
    result.displacements[i] = offset;
    offset += currentCount;
    remaining -= currentCount;
  }


  return result;
}

struct GatherOperation createGather(struct ScatterOperation *op, int outputElementsPerInputElement) {
  struct GatherOperation result = *(struct GatherOperation*)malloc(sizeof(struct GatherOperation));
  result.outputElementsPerInputElement = outputElementsPerInputElement;

  result.sendCounts = malloc(sizeof(int) * op->nodeCount);
  result.displacements = malloc(sizeof(int) * op->nodeCount);
  result.totalCount = op->totalCount * outputElementsPerInputElement;

  for(int i = 0; i < op->nodeCount; i++) {
    result.sendCounts[i] = op->sendCounts[i] * outputElementsPerInputElement;
    result.displacements[i] = op->displacements[i] * outputElementsPerInputElement;
  }
  return result;
}

void* allocateOutput(
  int rank,
  struct GatherOperation *op,
  int dataSize
) {
  int sendcount = op->sendCounts[rank];

  return malloc(dataSize * sendcount);
}

void* executeScatter(
  int rank,
  struct ScatterOperation *op,
  void *rootData,
  int* nodeInputCount,
  int dataSize,
  MPI_Datatype dataType,
  int rootRank,
  MPI_Comm comm
) {
  int recvcount = op->sendCounts[rank];

  void* nodeInput = malloc(dataSize * recvcount);
  *nodeInputCount = recvcount;
  
  MPI_Scatterv(
    rootData, op->sendCounts, op->displacements, dataType, 
    nodeInput, recvcount, dataType, rootRank, comm
  );

  return nodeInput;
}

void* executeGather(
  int rank,
  struct GatherOperation *op,
  void* nodeData,
  int dataSize,
  MPI_Datatype dataType,
  int rootRank,
  MPI_Comm comm
) {
  int recvcount = op->sendCounts[rank];
  
  void* rootData = NULL;

  if(rootRank == rank) {
    rootData = malloc(dataSize * op->totalCount);
  }
  
  MPI_Gatherv(
    nodeData, recvcount, dataType, 
    rootData, op->sendCounts, op->displacements, dataType, rootRank, comm
  );

  return rootData;
}

void main(int argc, char **argv)
{
  int rank, nodeCount;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nodeCount);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int outputElementsPerInputElement = 3;
  struct ScatterOperation sop = createScatter(N, nodeCount);
  struct GatherOperation gop = createGather(&sop, outputElementsPerInputElement);
  int *global_input = NULL;
  if (rank == 0)
  {
    global_input = malloc(sizeof(int) * sop.totalCount);
    for (int i = 0; i < sop.totalCount; i++)
    {
      global_input[i] = i;
    }
  }

  int nodeInputCount;
  int* nodeInput = (int*)executeScatter(rank, &sop, global_input, &nodeInputCount, sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);
  
  int* nodeOutput = allocateOutput(rank, &gop, sizeof(int));

  for(int i = 0; i < nodeInputCount; i++) {
    int j = i*gop.outputElementsPerInputElement;
    for(int k = 0; k < gop.outputElementsPerInputElement; k++) {
      nodeOutput[j + k] = k;
    }
  }

  int* global_output = executeGather(rank, &gop, nodeOutput, sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);

  printf("node %d: %d\n", rank, sop.sendCounts[rank]);


  for(int i = 0; i < nodeInputCount; i++) {
    printf("node %d: %d\n", rank, nodeInput[i]);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  sleep(1);

  if(rank == 0) {
    for(int i = 0; i < gop.totalCount; i++) {
      printf("%d: %d\n", i, global_output[i]);
    }
  }
  MPI_Finalize();
}