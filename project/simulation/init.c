void initializeTile(
  Tile* tile,
  long timesteps
) {
  if(tile->isOwnedByThisProcess) {
    tile->historicalDataCount = timesteps;
    tile->historicalData = (TileData*) calloc(timesteps, sizeof(TileData));

    tile->historicalData[0].rabbitsList = initList(sizeof(Rabbit), 150);
    tile->historicalData[0].foxesList = initList(sizeof(Fox), 2);

    if(!tile->isWaterTile) {
      for(long l = 0; l < 25; l++) {
        birthRabbit(tile, 0);
      }
      for(long l = 0; l < 2; l++) {
        birthFox(tile, 0);
      }
        
      tile->historicalData[0].vegetation = 1.0;
    }
    tile->rabbitMigrationsList = initList(sizeof(RabbitMigration), 100);
    tile->foxMigrationsList = initList(sizeof(FoxMigration), 100);
  }

}
