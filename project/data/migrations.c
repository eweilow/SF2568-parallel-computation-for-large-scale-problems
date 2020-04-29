void applyRabbitMigrations(
  TileGeometry *geometry,
  Tile *tile,
  long ts
) {
  List *migrationsList = &tile->rabbitMigrationsList;
  long len;
  RabbitMigration *migrations;
  list_read(migrationsList, &len, (void**)&migrations);
  for(long n = 0; n < len; n++) {
    char rabbitId[50];
    formatGlobalId(migrations[n].rabbit.id, rabbitId);

    long toTileIndex = migrations[n].toTile;
    Tile *toTile = geometry->tiles + (toTileIndex);

    List *toList = getRabbits(toTile, ts);

#if DEBUG_IPC
    printf("[DATA] rabbit.%s moves from %llu to %llu\n", rabbitId, tile->id, toTile->id);
#endif
    list_insert(toList, &migrations[n].rabbit);
  }
  list_clear(migrationsList);
}

void applyFoxMigrations(
  TileGeometry *geometry,
  Tile *tile,
  long ts
) {
  List *migrationsList = &tile->foxMigrationsList;
  long len;
  FoxMigration *migrations;
  list_read(migrationsList, &len, (void**)&migrations);
  for(long n = 0; n < len; n++) {
    char foxId[50];
    formatGlobalId(migrations[n].fox.id, foxId);

    long toTileIndex = migrations[n].toTile;
    Tile *toTile = geometry->tiles + (toTileIndex);

    List *toList = getFoxes(toTile, ts);

#if DEBUG_IPC
    printf("[DATA] fox.%s moves from %llu to %llu\n", foxId, tile->id, toTile->id);
#endif
    list_insert(toList, &migrations[n].fox);
  }
  list_clear(migrationsList);
}


void applyMigrationsOfTile(
  TileGeometry *geometry,
  Tile *tile,
  long ts
) {
  applyRabbitMigrations(geometry, tile, ts);
  applyFoxMigrations(geometry, tile, ts);
}

void applyMigrations(
  TileGeometry *geometry,
  long ts
) {
  for(long n = 0; n < geometry->tileCount; n++) {
    applyMigrationsOfTile(geometry, geometry->tiles + n, ts);
  }
}