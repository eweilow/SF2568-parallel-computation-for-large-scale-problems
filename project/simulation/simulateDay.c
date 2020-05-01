void simulateRabbitMigrations(
  Tile* tile, // The current tile
  long immediatelyAdjacentTileCount,
  Tile* immediatelyAdjacentTiles,
  long currentTimeStep)
{
  long draw;
  long index;
  long nRabbits = getRabbitCount(tile, currentTimeStep);
  long numberOfRabbitsToMigrate = numberOfRabbitsToMigrateAtEndOfDayRule(nRabbits);
  long remainingNumberOfRabbitsToMigrate = numberOfRabbitsToMigrate;
  // replace with drawing rabbit structs with indexes from rabbitsToMigrate from tile
  for(long i=0; i<numberOfRabbitsToMigrate; i++) {
    draw = getRandomInt(immediatelyAdjacentTileCount);
    Tile* drawnTile = &immediatelyAdjacentTiles[draw];
    index = getRandomInt(remainingNumberOfRabbitsToMigrate);
    migrateRabbit(tile, drawnTile, index, currentTimeStep);
    remainingNumberOfRabbitsToMigrate -=1;
  }
}

void simulateFoxMigrations(
  Tile* tile, // The current tile
  long immediatelyAdjacentTileCount,
  Tile* immediatelyAdjacentTiles,
  long adjacentToImmediatelyAdjacentTileCount,
  Tile* adjacentToImmediatelyAdjacentTiles,
  long currentTimeStep)
{
  printf("migrateFoxes\n");
  // remove from this tile and add to random tiles 1 or 2 steps away
  long draw;
  long index;
  long nFoxes = getFoxCount(tile, currentTimeStep);
  for(long i=0; i<nFoxes; i++) {
    draw = getRandomInt(1 + immediatelyAdjacentTileCount + adjacentToImmediatelyAdjacentTileCount);
    index = i;
    if (draw == immediatelyAdjacentTileCount + adjacentToImmediatelyAdjacentTileCount) {
      // Do nothing (fox stays in tile)
    } else if (draw < immediatelyAdjacentTileCount) {
      Tile* drawnTile = &immediatelyAdjacentTiles[draw];
      migrateFox(tile, drawnTile, index, currentTimeStep);
      i--; // List size decreased?????
    } else {
      draw = draw - immediatelyAdjacentTileCount;
      Tile* drawnTile = &adjacentToImmediatelyAdjacentTiles[draw];
      migrateFox(tile, drawnTile, index, currentTimeStep);
      i--; // List size decreased?????
    }
  }
}

void simulateMigrations(
  Tile* tile, // The current tile
  long immediatelyAdjacentTileCount,
  Tile* immediatelyAdjacentTiles,
  long adjacentToImmediatelyAdjacentTileCount,
  Tile* adjacentToImmediatelyAdjacentTiles,
  long currentTimeStep)
{
  simulateRabbitMigrations(tile,
    immediatelyAdjacentTileCount,
    immediatelyAdjacentTiles,
    currentTimeStep);

  simulateFoxMigrations(tile,
    immediatelyAdjacentTileCount,
    immediatelyAdjacentTiles,
    adjacentToImmediatelyAdjacentTileCount,
    adjacentToImmediatelyAdjacentTiles,
    currentTimeStep);
}


void simulateDay(
  TileGeometry *geometry,
  long ts
) {
  long nTiles = geometry->tileCount; // Number of tiles belonging to this process
  for (int i=0; i<nTiles; i++) {
    Tile *tileToUpdate = geometry->tiles + i; // Should only be tile belonging to this process
    tileLocalStartOfDayUpdates(tileToUpdate, ts);
  }
  for (int i=0; i<nTiles; i++) {
    Tile *tileToUpdate = geometry->tiles + i; // Should only be tile belonging to this process
    tileLocalMiddleOfDayUpdates(tileToUpdate, ts);
  }
  for (int i=0; i<nTiles; i++) {
    Tile *tileToUpdate = geometry->tiles + i; // Should only be tile belonging to this process
    // Migrations
  }
  for (int i=0; i<nTiles; i++) {
    Tile *tileToUpdate = geometry->tiles + i; // Should only be tile belonging to this process
    tileLocalEndOfDayUpdates(tileToUpdate, ts);
  }

    /*
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
        Tile* randTileTo = getRandomAdjacentTile(geometry, randTileFrom);
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
        Tile* randTileTo = getRandomAdjacentTile(geometry, randTileFrom);
        List* foxesList = getFoxes(randTileFrom, ts);
        long index = randomIndexOfList(foxesList);
        if(migrateFox(randTileFrom, randTileTo, index, ts)) { // These return true if they actually succeed, so we can try until they return true
          break;
        }
      }
    }
  }*/
}
