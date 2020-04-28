void simulateDay(
  TileGeometry *geometry,
  long ts
) {
  const long RABBIT_BIRTHS = 1;
  const long FOX_BIRTHS = 1;

  const long RABBIT_DEATHS = 1;
  const long FOX_DEATHS = 1;

  const long RABBIT_MIGRATIONS = 1;
  const long FOX_MIGRATIONS = 1;

  for(long k = 0; k < RABBIT_DEATHS; k++) {
    while(true) {
      Tile* randTile = getRandomTile(geometry);
      List* rabbitsList = getRabbits(randTile, ts);
      long index = randomIndexOfList(rabbitsList);
      if(killRabbit(randTile, index, ts)) { // These return true if they actually succeed, so we can try until they return true
        break;
      }
    }
  }

  for(long k = 0; k < FOX_DEATHS; k++) {
    while(true) {
      Tile* randTile = getRandomTile(geometry);
      List* foxesList = getFoxes(randTile, ts);
      long index = randomIndexOfList(foxesList);
      if(killFox(randTile, index, ts)) { // These return true if they actually succeed, so we can try until they return true
        break;
      }
    }
  }
  
  for(long k = 0; k < RABBIT_BIRTHS; k++) {
    Tile* randTile = getRandomTile(geometry);
    birthRabbit(randTile, ts);
  }

  for(long k = 0; k < FOX_BIRTHS; k++) {
    Tile* randTile = getRandomTile(geometry);
    birthFox(randTile, ts);
  }
  
  for(long k = 0; k < RABBIT_MIGRATIONS; k++) {
    while(true) {
      Tile* randTileFrom = getRandomTile(geometry);
      Tile* randTileTo = getRandomTile(geometry);
      List* rabbitsList = getRabbits(randTileFrom, ts);
      long index = randomIndexOfList(rabbitsList);
      if(migrateRabbit(randTileFrom, randTileTo, index, ts)) { // These return true if they actually succeed, so we can try until they return true
        break;
      }
    }
  }

  for(long k = 0; k < FOX_MIGRATIONS; k++) {
    while(true) {
      Tile* randTileFrom = getRandomTile(geometry);
      Tile* randTileTo = getRandomTile(geometry);
      List* foxesList = getFoxes(randTileFrom, ts);
      long index = randomIndexOfList(foxesList);
      if(migrateFox(randTileFrom, randTileTo, index, ts)) { // These return true if they actually succeed, so we can try until they return true
        break;
      }
    }
  }
}