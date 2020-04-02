typedef struct {
  long rabbitCount;
  long foxCount;
  double vegetation;
} TileData;

typedef struct {
  double x;
  double y;

  bool isWaterTile;

  struct TileData* historicalData;
} Tile;
