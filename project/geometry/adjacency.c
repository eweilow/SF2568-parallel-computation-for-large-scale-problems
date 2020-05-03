void printSquare(long val) {
  if(val >= 0) {
    printf("%4ld", val);
  } else {
    printf("%4s", "");
  }
}

long computeAdjacencyWithOffset(
  long px,
  long py,
  long i,
  long j,
  long processesWide, 
  long processesHigh
) {
  if(px < 0) {
    return -1;
  }
  if(py < 0) {
    return -1;
  }
  if(px >= processesWide) {
    return -1;
  }
  if(py >= processesHigh) {
    return -1;
  }

  long x = px + i;
  long y = py + j;

  if(x < 0) {
    return -1;
  }
  if(y < 0) {
    return -1;
  }
  if(x >= processesWide) {
    return -1;
  }
  if(y >= processesHigh) {
    return -1;
  }
  return getProcessIndex(x, y, processesWide, processesHigh);
}

ProcessAdjacency getAdjacency(
  long process, 
  long processesWide, 
  long processesHigh
) {
  long px, py;
  getProcessLocation(process, processesWide, processesHigh, &px, &py);

  ProcessAdjacency adjacency;

  adjacency.col = px;
  adjacency.row = py;

  adjacency.upLeft = computeAdjacencyWithOffset(px, py, -1, -1, processesWide, processesHigh);
  adjacency.up = computeAdjacencyWithOffset(px, py, 0, -1, processesWide, processesHigh);
  adjacency.upRight = computeAdjacencyWithOffset(px, py, 1, -1, processesWide, processesHigh);

  adjacency.left = computeAdjacencyWithOffset(px, py, -1, 0, processesWide, processesHigh);
  adjacency.current = process;
  adjacency.right = computeAdjacencyWithOffset(px, py, 1, 0, processesWide, processesHigh);

  adjacency.downLeft = computeAdjacencyWithOffset(px, py, -1, 1, processesWide, processesHigh);
  adjacency.down = computeAdjacencyWithOffset(px, py, 0, 1, processesWide, processesHigh);
  adjacency.downRight = computeAdjacencyWithOffset(px, py, 1, 1, processesWide, processesHigh);

  adjacency.list[0] = adjacency.upLeft;
  adjacency.list[1] = adjacency.up;
  adjacency.list[2] = adjacency.upRight;
  adjacency.list[3] = adjacency.left;
  adjacency.list[4] = adjacency.right;
  adjacency.list[5] = adjacency.downLeft;
  adjacency.list[6] = adjacency.down;
  adjacency.list[7] = adjacency.downRight;
  
  #if DEBUG_PROCESS_ADJACENCY
    printf("\n");
    printSquare(adjacency.upLeft);
    printf(" | ");
    printSquare(adjacency.up);
    printf(" | ");
    printSquare(adjacency.upRight);
    printf("\n");

    printSquare(adjacency.left);
    printf(" | ");
    printSquare(process);
    printf(" | ");
    printSquare(adjacency.right);
    printf("\n");

    printSquare(adjacency.downLeft);
    printf(" | ");
    printSquare(adjacency.down);
    printf(" | ");
    printSquare(adjacency.downRight);
    printf("\n");
    printf("\n");
  #endif

  return adjacency;
}