typedef struct {
  long birthDay;
} Rabbit;

#define MEALS_LENGTH 4

typedef struct {
  long birthDay;

  long nextMealIndex; // The index where to write the next meal
  long mealTimeStamps[MEALS_LENGTH];
} Fox;

typedef struct {
  List rabbitsList;
  List foxesList;
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

typedef struct {
  long tileCount;
  Tile* tiles;
} TileGeometry;
