typedef struct {
  long birthDay;
  u_int64_t id;
} Rabbit;

typedef struct {
  long birthDay;
  u_int64_t id;

  double hunger;
} Fox;

typedef struct {
  Rabbit rabbit;
  long fromTile;
  long toTile;
} RabbitMigration;

typedef struct {
  Fox fox;
  long fromTile;
  long toTile;
} FoxMigration;

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
  u_int64_t id;

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
