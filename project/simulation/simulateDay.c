void simulateDay(
  TileGeometry *geometry,
  long ts
) {
  const long MAXITER = 10;

  const long RABBIT_BIRTHS = 100;
  const long FOX_BIRTHS = 100;

  const long RABBIT_DEATHS = 100;
  const long FOX_DEATHS = 100;

  const long RABBIT_MIGRATIONS = 10;
  const long FOX_MIGRATIONS = 10;

  for(long k = 0; k < RABBIT_DEATHS; k++) {
    long i = 0;
    while(++i <= MAXITER) {
      Tile* randTile = getRandomTile(geometry);
      List* rabbitsList = getRabbits(randTile, ts);
      long index = randomIndexOfList(rabbitsList);
      if(killRabbit(randTile, index, ts)) { // These return true if they actually succeed, so we can try until they return true
        break;
      }
    }
  }

  for(long k = 0; k < FOX_DEATHS; k++) {
    long i = 0;
    while(++i <= MAXITER) {
      Tile* randTile = getRandomTile(geometry);
      List* foxesList = getFoxes(randTile, ts);
      long index = randomIndexOfList(foxesList);
      if(killFox(randTile, index, ts)) { // These return true if they actually succeed, so we can try until they return true
        break;
      }
    }
  }
  
  for(long k = 0; k < RABBIT_BIRTHS; k++) {
    long i = 0;
    while(++i <= MAXITER) {
      Tile* randTile = getRandomTile(geometry);
      if(birthRabbit(randTile, ts)) { // These return true if they actually succeed, so we can try until they return true
        break;
      }
    }
  }

  for(long k = 0; k < FOX_BIRTHS; k++) {
    long i = 0;
    while(++i <= MAXITER) {
      Tile* randTile = getRandomTile(geometry);
      if(birthFox(randTile, ts)) { // These return true if they actually succeed, so we can try until they return true
        break;
      }
    }
  }
  
  for(long k = 0; k < RABBIT_MIGRATIONS; k++) {
    long i = 0;
    while(++i <= MAXITER) {
      Tile* randTileFrom = getRandomTile(geometry);
      Tile* randTileTo = getRandomAdjacentTile(geometry, randTileFrom);
      if(randTileTo == NULL) {
        break;
      }
      List* rabbitsList = getRabbits(randTileFrom, ts);
      long index = randomIndexOfList(rabbitsList);
      if(migrateRabbit(randTileFrom, randTileTo, index, ts)) { // These return true if they actually succeed, so we can try until they return true
        break;
      }
    }
  }

  for(long k = 0; k < FOX_MIGRATIONS; k++) {
    long i = 0;
    while(++i <= MAXITER) {
      Tile* randTileFrom = getRandomTile(geometry);
      Tile* randTileTo = getRandomAdjacentTile(geometry, randTileFrom);
      if(randTileTo == NULL) {
        break;
      }
      List* foxesList = getFoxes(randTileFrom, ts);
      long index = randomIndexOfList(foxesList);
      if(migrateFox(randTileFrom, randTileTo, index, ts)) { // These return true if they actually succeed, so we can try until they return true
        break;
      }
    }
  }
}