void simulateRabbitMigrations(
  TileGeometry *geometry,
  Tile* tile,
  long ts)
{
  printf("migrateRabbits\n");
  // remove from this tile and add to random tiles 1 or 2 steps away
  long nRabbits = getRabbitCount(tile, ts);
  for(long i=0; i < nRabbits; i++) {
    Tile* nextTile = getRandomRabbitTile(geometry, tile);
    migrateRabbit(tile, nextTile, i, ts);
  }
}

void simulateFoxMigrations(
  TileGeometry *geometry,
  Tile* tile,
  long ts)
{
  printf("migrateFoxes\n");
  // remove from this tile and add to random tiles 1 or 2 steps away
  long nFoxes = getFoxCount(tile, ts);
  for(long i=0; i < nFoxes; i++) {
    Tile* nextTile = getRandomFoxTile(geometry, tile);
    migrateFox(tile, nextTile, i, ts);
  }
}

void simulateMigrations(
  TileGeometry *geometry,
  Tile* tile, 
  long ts
)
{
  simulateRabbitMigrations(geometry, tile, ts);
  simulateFoxMigrations(geometry, tile, ts);
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
    simulateMigrations(
      geometry, 
      tileToUpdate,
      ts
    );
  }
  for (int n=0; n < geometry->ownTileCount; n++) {
    long i = geometry->ownTileIndices[n];
    Tile *tileToUpdate = geometry->tiles + i; // Should only be tile belonging to this process
    tileLocalEndOfDayUpdates(tileToUpdate, ts);
  }
}
