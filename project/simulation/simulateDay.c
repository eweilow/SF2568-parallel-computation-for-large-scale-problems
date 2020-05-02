void simulateRabbitMigrations(
  TileGeometry *geometry,
  Tile* tile,
  long ts)
{
  // remove from this tile and add to random tiles 1 or 2 steps away
  long nRabbits = getRabbitCount(tile, ts);
  long rabbitsToMigrate = numberOfRabbitsToMigrateAtEndOfDayRule(nRabbits);
  for(long i=0; i < rabbitsToMigrate; i++) {
    long nextRabbit = getRandomInt(nRabbits);
    Tile* nextTile = getRandomRabbitTile(geometry, tile);
    migrateRabbit(tile, nextTile, nextRabbit, ts);
    --nRabbits;
  }
}

void simulateFoxMigrations(
  TileGeometry *geometry,
  Tile* tile,
  long ts)
{
  // remove from this tile and add to random tiles 1 or 2 steps away
  long nFoxes = getFoxCount(tile, ts);
  for(long i=0; i < nFoxes; i++) {
    Tile* nextTile = getRandomFoxTile(geometry, tile);
    migrateFox(tile, nextTile, 0, ts);
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


#define USE_START_OF_DAY_UPDATES 1
#define USE_MIDDLE_OF_DAY_UPDATES 1
#define USE_MIGRATION_UPDATES 1
#define USE_END_OF_DAY_UPDATES 1

void simulateDay(
  TileGeometry *geometry,
  long ts
) {
  #if USE_START_OF_DAY_UPDATES
  for (int n=0; n < geometry->ownTileCount; n++) {
    long i = geometry->ownTileIndices[n];
    Tile *tileToUpdate = geometry->tiles + i; // Should only be tile belonging to this process
    tileLocalStartOfDayUpdates(tileToUpdate, ts);
  }
  #endif 
  #if USE_MIDDLE_OF_DAY_UPDATES
  for (int n=0; n < geometry->ownTileCount; n++) {
    long i = geometry->ownTileIndices[n];
    Tile *tileToUpdate = geometry->tiles + i; // Should only be tile belonging to this process
    tileLocalMiddleOfDayUpdates(tileToUpdate, ts);
  }
  #endif 
  #if USE_MIGRATION_UPDATES
  for (int n=0; n < geometry->ownTileCount; n++) {
    long i = geometry->ownTileIndices[n];
    Tile *tileToUpdate = geometry->tiles + i; // Should only be tile belonging to this process
    simulateMigrations(
      geometry, 
      tileToUpdate,
      ts
    );
  }
  #endif 
  #if USE_END_OF_DAY_UPDATES
  for (int n=0; n < geometry->ownTileCount; n++) {
    long i = geometry->ownTileIndices[n];
    Tile *tileToUpdate = geometry->tiles + i; // Should only be tile belonging to this process
    tileLocalEndOfDayUpdates(tileToUpdate, ts);
  }
  #endif 
}
