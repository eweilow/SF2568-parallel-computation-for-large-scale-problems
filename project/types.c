typedef struct {
  long rabbitCount;
  long foxCount;
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

typedef struct {
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