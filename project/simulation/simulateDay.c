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
  for (int n=0; n < geometry->ownTileCount; n++) {
    long i = geometry->ownTileIndices[n];
    Tile *tileToUpdate = geometry->tiles + i; // Should only be tile belonging to this process
    tileLocalStartOfDayUpdates(tileToUpdate, ts);
  }
  for (int n=0; n < geometry->ownTileCount; n++) {
    long i = geometry->ownTileIndices[n];
    Tile *tileToUpdate = geometry->tiles + i; // Should only be tile belonging to this process
    tileLocalMiddleOfDayUpdates(tileToUpdate, ts);
  }
  for (int n=0; n < geometry->ownTileCount; n++) {
    long i = geometry->ownTileIndices[n];
    Tile *tileToUpdate = geometry->tiles + i; // Should only be tile belonging to this process
    // Migrations
  }
  for (int n=0; n < geometry->ownTileCount; n++) {
    long i = geometry->ownTileIndices[n];
    Tile *tileToUpdate = geometry->tiles + i; // Should only be tile belonging to this process
    tileLocalEndOfDayUpdates(tileToUpdate, ts);
  }
}
