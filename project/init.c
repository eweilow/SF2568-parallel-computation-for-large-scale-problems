void initializeTile(
  Tile* tile,
  long timesteps
) {
  tile->historicalData = (TileData*) calloc(timesteps, sizeof(TileData));

  TileData initialData;

  initialData.rabbitCount = 13;
  initialData.foxCount = 7;
  initialData.vegetation = 0.25;

  tile->historicalData[0] = initialData;
}