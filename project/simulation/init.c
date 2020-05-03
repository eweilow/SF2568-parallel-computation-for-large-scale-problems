void initializeTile(
  Tile* tile,
  long timesteps
) {
  if(tile->isOwnedByThisProcess) {
    tile->historicalDataCount = timesteps;
    tile->historicalData = (TileData*) calloc(timesteps, sizeof(TileData));

    tile->historicalData[0].rabbitsList = initList(sizeof(Rabbit), 500);
    tile->historicalData[0].foxesList = initList(sizeof(Fox), 100);

    if(!tile->isWaterTile) {
      #if GEOM_CASE == 0
        long rabbitsCount = 100;
        long foxesCount = 2;
        tile->historicalData[0].vegetation = 1.0;
      #elif GEOM_CASE == 1
        long rabbitsCount = tile->x >=  ((double)GEOM_W - 0.5) && tile->y >= ((double)GEOM_H - 0.5) ? 400 : 20;
        long foxesCount = tile->x <= 1.5 && tile->y <= 1.5 ? 20 : 0;
        tile->historicalData[0].vegetation = 0.3;
      #elif GEOM_CASE == 2
        long rabbitsCount = 2;
        long foxesCount = 0;
        tile->historicalData[0].vegetation = 0.5;
      #endif
      
      for(long l = 0; l < rabbitsCount; l++) {
        birthRabbit(tile, 0);
      }
      for(long l = 0; l < foxesCount; l++) {
        birthFox(tile, 0);
      }
        
    }
    tile->rabbitMigrationsList = initList(sizeof(RabbitMigration), 100);
    tile->foxMigrationsList = initList(sizeof(FoxMigration), 100);
  }
}
