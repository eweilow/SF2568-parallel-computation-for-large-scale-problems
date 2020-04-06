#include "./utilities.c"


void migrateRabbit(Tile* from, Tile* to, long timestep){
  from->historicalData[timestep].rabbitCount -= 1;
  to->historicalData[timestep + 1].rabbitCount += 1;
}

void migrateRabbits(
  Tile* tile, // The current tile
  long immediatelyAdjacentTileCount,
  Tile* immediatelyAdjacentTiles,
  long adjacentToImmediatelyAdjacentTileCount,
  Tile* adjacentToImmediatelyAdjacentTiles,
  long currentTimeStep
) {
  printf("migrateRabbits\n");
  // remove from this tile and add to random immediately adjacent tiles
  long draw;
  long nRabbits = tile->historicalData[currentTimeStep].rabbitCount;
  for(long i=0; i<nRabbits; i++) {
    // draw random number
    // find corresponding tile
    // add to tile historicalData of this step
    draw = getRandomInt(immediatelyAdjacentTileCount);
    printf("%d of %d\n", draw, immediatelyAdjacentTileCount - 1);
    Tile* drawnTile = &immediatelyAdjacentTiles[draw];
    migrateRabbit(tile, drawnTile, currentTimeStep);
  }
}

void migrateFox(Tile* from, Tile* to, long timestep){
  from->historicalData[timestep].foxCount -= 1;
  to->historicalData[timestep + 1].foxCount += 1;
}

void migrateFoxes(
  Tile* tile, // The current tile
  long immediatelyAdjacentTileCount,
  Tile* immediatelyAdjacentTiles,
  long adjacentToImmediatelyAdjacentTileCount,
  Tile* adjacentToImmediatelyAdjacentTiles,
  long currentTimeStep
) {
  printf("migrateFoxes\n");
  // remove from this tile and add to random tiles 1 or 2 steps away
  long draw;
  long nFoxes = tile->historicalData[currentTimeStep].foxCount;
  for(long i=0; i<nFoxes; i++) {
    // draw random number
    // find corresponding tile
    // add to tile historicalData of this step
    draw = getRandomInt(immediatelyAdjacentTileCount + adjacentToImmediatelyAdjacentTileCount);
    if (draw < immediatelyAdjacentTileCount) {
      Tile* drawnTile = &immediatelyAdjacentTiles[draw];
      migrateFox(tile, drawnTile, currentTimeStep);
    } else {
      draw = draw - immediatelyAdjacentTileCount;
      Tile* drawnTile = &adjacentToImmediatelyAdjacentTiles[draw];
      migrateFox(tile, drawnTile, currentTimeStep);
    }

  }
}

void updateTile(
  Tile* tile, // The current tile
  long immediatelyAdjacentTileCount,
  Tile* immediatelyAdjacentTiles,
  long adjacentToImmediatelyAdjacentTileCount,
  Tile* adjacentToImmediatelyAdjacentTiles,
  long currentTimeStep
) {
  // Other stuff

  // Migrate stuff
  migrateRabbits(
    tile,
    immediatelyAdjacentTileCount,
    immediatelyAdjacentTiles,
    adjacentToImmediatelyAdjacentTileCount,
    adjacentToImmediatelyAdjacentTiles,
    currentTimeStep
  );

  migrateFoxes(
    tile,
    immediatelyAdjacentTileCount,
    immediatelyAdjacentTiles,
    adjacentToImmediatelyAdjacentTileCount,
    adjacentToImmediatelyAdjacentTiles,
    currentTimeStep
  );
}

//void saveState(Tile* tile, long timestep){
//  tile.rabbitCount = tile.historicalData[timestep].rabbitCount;
//  tile.foxCount = tile.historicalData[timestep].foxCount;
//  tile.vegetation = tile.historicalData[timestep].vegetation;
//}
