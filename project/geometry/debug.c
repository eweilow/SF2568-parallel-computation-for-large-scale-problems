void debugGeometryAdjacency(
  TileGeometry* geometry
){
  Tile* tiles = geometry->tiles;
  for(long i = 0; i < geometry->tilesWide; i++) {
    for(long j = 0; j < geometry->tilesHigh; j++) {
      long self = i*geometry->tilesHigh + j;
      printf("id = %llu, x = %.2f, y = %.2f\n water: %d\n", tiles[self].id, tiles[self].x, tiles[self].y, tiles[self].isWaterTile);

      for(long n = 0; n < tiles[self].adjacency.immediatelyAdjacentTileIndicesCount; n++) {
        long other = tiles[self].adjacency.immediatelyAdjacentTileIndices[n];
        printf(" (%llu -> %llu, %ld): (%.2f, %.2f) -> (%.2f, %.2f)\n", tiles[self].id, tiles[other].id, other, tiles[self].x, tiles[self].y, tiles[other].x, tiles[other].y);
      }
      for(long n = 0; n < tiles[self].adjacency.indirectlyAdjacentTileIndicesCount; n++) {
        long other = tiles[self].adjacency.indirectlyAdjacentTileIndices[n];
        printf(" (%llu -> %llu, %ld): (%.2f, %.2f) /'''\\> (%.2f, %.2f)\n", tiles[self].id, tiles[other].id, other, tiles[self].x, tiles[self].y, tiles[other].x, tiles[other].y);
      }
    }
  }
}


void debugRabbit(
  Rabbit *rabbit
) {
  printf("Rabbit %ld, born: %ld\n", rabbit->id, rabbit->birthDay);
}

void debugFox(
  Fox *fox
) {
  printf("Fox %ld, born: %ld, hunger: %.4lf\n", fox->id, fox->birthDay, fox->hunger);
}

void debugTileRabbits(
  Tile *tile,
  long timestep
) {
  TileData data = tile->historicalData[timestep];

  long rabbitsCount;
  Rabbit* rabbits;
  list_read(&data.rabbitsList, &rabbitsCount, (void**)&rabbits);

  for(long l = 0; l < rabbitsCount; l++) {
    char rabbitId[50];
    formatGlobalId(rabbits[l].id, rabbitId);
    printf("  %5llu %7s %5ld\n", tile->id, rabbitId, rabbits[l].birthDay);
  }
}

void debugTileFoxes(
  Tile *tile,
  long timestep
) {
  TileData data = tile->historicalData[timestep];

  long foxesCount;
  Fox* foxes;
  list_read(&data.foxesList, &foxesCount, (void**)&foxes);

  for(long l = 0; l < foxesCount; l++) {
    char foxId[50];
    formatGlobalId(foxes[l].id, foxId);
    printf("  %5llu %7s %5ld | %5.4lf\n", tile->id, foxId, foxes[l].birthDay, foxes[l].hunger);
  }
}

void debugTile(
  Tile *tile,
  long timestep
) {

  TileData data = tile->historicalData[timestep];
  printf("%5llu %8ld (%5.2f, %5.2f): %8d %8d %8.2f\n",
    tile->id,
    tile->process,
    tile->x,
    tile->y,
    data.rabbitsList.elementCount,
    data.foxesList.elementCount,
    data.vegetation
  );
}

void debugTiles(
  TileGeometry* geometry,
  long timestep
) {

  printf("\n");
  printf("== tiles at timestep %ld ==\n", timestep);
  printf("%5s %8s (%5s, %5s): %8s %8s %8s\n", "id", "process", "x", "y", "rabbits", "foxes", "veg.");
  for(long n = 0; n < geometry->ownTileCount; n++) {
    long i = geometry->ownTileIndices[n];
    debugTile(geometry->tiles + i, timestep);
  }

  #if DEBUG_INDIVIDUAL_ANIMALS
    printf("== all rabbits at timestep %ld ==\n", timestep);
    printf("  %5s %7s %5s\n", "tile", "id", "born");
    for(long n = 0; n < geometry->ownTileCount; n++) {
      long i = geometry->ownTileIndices[n];
      debugTileRabbits(geometry->tiles + i, timestep);
    }

    printf("== all foxes at timestep %ld ==\n", timestep);
    printf("  %5s %7s %5s | %s \n", "tile", "id", "born", "hunger");
    for(long n = 0; n < geometry->ownTileCount; n++) {
      long i = geometry->ownTileIndices[n];
      debugTileFoxes(geometry->tiles + i, timestep);
    }
    printf("\n");
  #endif
}
