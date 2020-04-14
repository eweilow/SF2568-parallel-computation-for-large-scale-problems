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

typedef struct {
  long tileCount;
  Tile* tiles;
} TileGeometry;
