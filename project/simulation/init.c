void initializeTile(
  Tile* tile,
  long timesteps
) {
  tile->historicalData = (TileData*) calloc(timesteps, sizeof(TileData));

  tile->historicalData[0].rabbitsList = initList(sizeof(Rabbit), 0);
  for(long l = 0; l < 13; l++) {
    birthRabbit(tile, 0);
  }

  tile->historicalData[0].foxesList = initList(sizeof(Fox), 0);
  for(long l = 0; l < 7; l++) {
    birthFox(tile, 0);
  }
    
  tile->historicalData[0].vegetation = 0.25;

  tile->rabbitMigrationsList = initList(sizeof(RabbitMigration), 0);
  tile->foxMigrationsList = initList(sizeof(FoxMigration), 0);
}
