void debugGeometryAdjacency(
  TileGeometry* geometry
){
  Tile* tiles = geometry->tiles;
  for(long i = 0; i < geometry->tilesWide; i++) {
    for(long j = 0; j < geometry->tilesHigh; j++) {
      long self = i*geometry->tilesWide + j;
      printf("x = %.2f, y = %.2f\n water: %d\n", tiles[self].x, tiles[self].y, tiles[self].isWaterTile);

      for(long n = 0; n < tiles[self].adjacency.immediatelyAdjacentTileIndicesCount; n++) {
        long other = tiles[self].adjacency.immediatelyAdjacentTileIndices[n];
        printf(" (%.2f, %.2f) -> (%.2f, %.2f)\n", tiles[self].x, tiles[self].y, tiles[other].x, tiles[other].y);
      }
      for(long n = 0; n < tiles[self].adjacency.indirectlyAdjacentTileIndicesCount; n++) {
        long other = tiles[self].adjacency.indirectlyAdjacentTileIndices[n];
        printf(" (%.2f, %.2f) /'''\\> (%.2f, %.2f)\n", tiles[self].x, tiles[self].y, tiles[other].x, tiles[other].y);
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
    printf("  %5ld %5ld %5ld\n", tile->id, rabbits[l].id, rabbits[l].birthDay);
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
    printf("  %5ld %5ld %5ld | %5.4lf\n", tile->id, foxes[l].id, foxes[l].birthDay, foxes[l].hunger);
  }
}

void debugTile(
  Tile *tile,
  long timestep
) {

  TileData data = tile->historicalData[timestep];
  printf("%5ld (%5.2f, %5.2f): %8d %8d %8.2f\n",
    tile->id,
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
  printf("%5s (%5s, %5s): %8s %8s %8s\n", "id", "x", "y", "rabbits", "foxes", "veg.");
  for(long n = 0; n < geometry->tileCount; n++) {
    debugTile(geometry->tiles + n, timestep);
  }

  #if DEBUG_INDIVIDUAL_ANIMALS
    printf("== all rabbits at timestep %ld ==\n", timestep);
    printf("  %5s %5s %5s\n", "tile", "id", "born");
    for(long n = 0; n < geometry->tileCount; n++) {
      debugTileRabbits(geometry->tiles + n, timestep);
    }

    printf("== all foxes at timestep %ld ==\n", timestep);
    printf("  %5s %5s %5s | %s \n", "tile", "id", "born", "hunger");
    for(long n = 0; n < geometry->tileCount; n++) {
      debugTileFoxes(geometry->tiles + n, timestep);
    }
    printf("\n");
  #endif
}