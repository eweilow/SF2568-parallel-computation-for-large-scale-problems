typedef struct {
  long birthDay;
  long id;
} Rabbit;

typedef struct {
  long birthDay;
  long id;

  double hunger;
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
  long id;

  bool isWaterTile;

  long historicalDataCount;
  TileData* historicalData;

  TileAdjacency adjacency;
} Tile;

typedef struct {
  long tilesWide; 
  long tilesHigh; 
  long tileCount;
  Tile* tiles;
} TileGeometry;
