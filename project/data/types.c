typedef struct {
  long birthDay;
  u_int64_t id;
} Rabbit;

typedef struct {
  long birthDay;
  u_int64_t id;

  double hunger;
  double extraMeals;
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

#if USE_REDUCED_OUTPUT
  long foxCount;
  long rabbitCount;

  double totalFoxHunger;
  long totalFoxAge;
  long totalRabbitAge;
  
  long maxFoxAge;
  long maxRabbitAge;
#endif
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

#define ADJACENT_PROCESSES 8

typedef struct {
  long col;
  long row;

  long upLeft;
  long up;
  long upRight;
  long left;
  long current;
  long right;
  long downLeft;
  long down;
  long downRight;

  long list[ADJACENT_PROCESSES];
} ProcessAdjacency;