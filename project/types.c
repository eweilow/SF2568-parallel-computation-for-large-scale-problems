typedef struct {
  long birthDay;
} Rabbit;

typedef struct {
  long birthDay;
  long* mealTimeStamps;
} Fox;

typedef struct {
  long rabbitCount;
  Rabbit* rabbits;
  long foxCount;
  Fox* foxes;
  double vegetation;
} TileData;

typedef struct {
  // One step away
  long immediatelyAdjacentTileIndicesCount;
  long* immediatelyAdjacentTileIndices;

  // Two steps away
  long indirectlyAdjacentTileIndicesCount;
  long* indirectlyAdjacentTileIndices;
} TileAdjacency;

typedef struct Tile{
  double x;
  double y;

  bool isWaterTile;

  long historicalDataCount;
  TileData* historicalData;

  TileAdjacency adjacency;
} Tile;

Fox getFoxFromTile(Tile* tile, int index, int timestep){
  Fox* foxes = tile->historicalData[timestep].foxes;
//  if (tile.historicalData[timestep].foxCount == 0)
//    return -1;

  return foxes[index];
}

Fox removeFoxFromTile(Tile* tile, int index, int timestep){
  Fox* foxes = tile->historicalData[timestep].foxes;
//  if (tile.historicalData[timestep].foxCount == 0)
//    return -1;
  Fox fox = foxes[index];
  //free(foxes[index]); // = NULL;
  tile->historicalData[timestep].foxCount -= 1;
  return fox;
}

void addFoxToTile(Tile* tile, Fox fox, int timestep){
  TileData* currentTileData = &tile->historicalData[timestep];
  int freeIndex = currentTileData->foxCount; // replace with, getFreeIndex
  currentTileData->foxes[freeIndex] = fox;
  currentTileData->foxCount += 1;
}

typedef struct {
  long tileCount;
  Tile* tiles;
} TileGeometry;
