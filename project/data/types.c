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
  long toTile;
} RabbitMigration;

typedef struct {
  Fox fox;
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

  long process;
  bool isOwnedByThisProcess;

  bool isWaterTile;

  long historicalDataCount;
  TileData* historicalData;

  List rabbitMigrationsList; // List of all rabbits migrating out from this tile at the end of the current time step
  List foxMigrationsList; // List of all foxes migrating out from this tile at the end of the current time step

  TileAdjacency adjacency;
} Tile;

typedef struct {
  double tileSize;
  
  long tilesWide; 
  long tilesHigh; 
  long tileCount;
  Tile* tiles;

  long ownTileCount;
  long* ownTileIndices;
} TileGeometry;
