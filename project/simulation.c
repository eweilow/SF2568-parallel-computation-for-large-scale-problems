#include "./utilities.c"


void migrateRabbit(Tile* from, Tile* to, long timestep){
  from->historicalData[timestep].rabbitCount -= 1;
  to->historicalData[timestep].rabbitCount += 1;
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
  long numberOfRabbitsToMigrate = nRabbits / 5;
  long* rabbitsToMigrate = getRandomNumbers(numberOfRabbitsToMigrate, nRabbits);
  // replace with drawing rabbit structs with indexes from rabbitsToMigrate from tile
  for(long i=0; i<numberOfRabbitsToMigrate; i++) {
    // draw random number
    // find corresponding tile
    // add to tile historicalData of this step
    draw = getRandomInt(immediatelyAdjacentTileCount);
    //printf("%d of %d\n", draw, immediatelyAdjacentTileCount - 1);
    Tile* drawnTile = &immediatelyAdjacentTiles[draw];
    migrateRabbit(tile, drawnTile, currentTimeStep);
  }
}

void migrateFox(Tile* from, Tile* to, long timestep){
  from->historicalData[timestep].foxCount -= 1;
  to->historicalData[timestep].foxCount += 1;
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
    // add to tile.historicalData of this step
    draw = getRandomInt(1 + immediatelyAdjacentTileCount + adjacentToImmediatelyAdjacentTileCount);
    if (draw == immediatelyAdjacentTileCount + adjacentToImmediatelyAdjacentTileCount) {
      // Do nothing (fox stays in tile)
    } else if (draw < immediatelyAdjacentTileCount) {
      Tile* drawnTile = &immediatelyAdjacentTiles[draw];
      migrateFox(tile, drawnTile, currentTimeStep);
    } else {
      draw = draw - immediatelyAdjacentTileCount;
      Tile* drawnTile = &adjacentToImmediatelyAdjacentTiles[draw];
      migrateFox(tile, drawnTile, currentTimeStep);
    }
  }
}

void vegetationGrowth(Tile* tile, int currentTimeStep) {
  double vegetationStart = tile->historicalData[currentTimeStep - 1].vegetation;
  int rabbitCountStart = tile->historicalData[currentTimeStep - 1].rabbitCount;
  double vegetationEnd = 1.1*(vegetationStart - 0.001*rabbitCountStart);
  vegetationEnd = vegetationEnd > 1.0 ? 1.0 : vegetationEnd;
  vegetationEnd = vegetationEnd < 0.1 ? 0.1 : vegetationEnd;
  tile->historicalData[currentTimeStep].vegetation = vegetationEnd;
}

void startDataOfNewDay(Tile* tile, int currentTimeStep) {
  long rabbitCount = tile->historicalData[currentTimeStep - 1].rabbitCount;
  long foxCount = tile->historicalData[currentTimeStep - 1].foxCount;
  double vegetationCount = tile->historicalData[currentTimeStep - 1].vegetation;

  TileData newDayTileData;

  newDayTileData.rabbitCount = rabbitCount;
  newDayTileData.foxCount = foxCount;
  newDayTileData.vegetation = vegetationCount;

  tile->historicalData[currentTimeStep] = newDayTileData;
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
  startDataOfNewDay(tile, currentTimeStep);
  vegetationGrowth(tile, currentTimeStep);
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
