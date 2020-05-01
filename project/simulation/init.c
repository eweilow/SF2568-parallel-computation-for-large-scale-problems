void initializeTile(
  Tile* tile,
  long timesteps
) {
  if(tile->isOwnedByThisProcess) {
    tile->historicalDataCount = timesteps;
    tile->historicalData = (TileData*) calloc(timesteps, sizeof(TileData));

    tile->historicalData[0].rabbitsList = initList(sizeof(Rabbit), 40);
    tile->historicalData[0].foxesList = initList(sizeof(Fox), 40);

    if(!tile->isWaterTile) {
      for(long l = 0; l < 40; l++) {
        birthRabbit(tile, 0);
      }
      for(long l = 0; l < 40; l++) {
        birthFox(tile, 0);
      }
        
      tile->historicalData[0].vegetation = 0.25;
    }
    tile->rabbitMigrationsList = initList(sizeof(RabbitMigration), 100);
    tile->foxMigrationsList = initList(sizeof(FoxMigration), 100);
  }

}
