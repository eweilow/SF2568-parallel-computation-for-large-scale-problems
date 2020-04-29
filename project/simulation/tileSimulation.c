void vegetationGrowth(Tile* tile, int currentTimeStep) {
  double vegetationStart = tile->historicalData[currentTimeStep - 1].vegetation;
  long rabbitCountStart = (long) getRabbitCount(tile, currentTimeStep - 1);
  double vegetationEnd = vegetationAtEndOfDayRule(vegetationStart, rabbitCountStart);
  tile->historicalData[currentTimeStep].vegetation = vegetationEnd;
}

void tileLocalStartOfDayUpdates(Tile* tile, long currentTimeStep) {
  startDataOfNewDay(tile, currentTimeStep);
}

void tileLocalEndOfDayUpdates(Tile* tile, long currentTimeStep) {
  vegetationGrowth(tile, currentTimeStep);

}

void updateTile(
  Tile* tile, // The current tile
  long immediatelyAdjacentTileCount,
  Tile* immediatelyAdjacentTiles,
  long adjacentToImmediatelyAdjacentTileCount,
  Tile* adjacentToImmediatelyAdjacentTiles,
  long currentTimeStep
) {
  // Tile local updates start of day
  tileLocalStartOfDayUpdates(tile, currentTimeStep);

  // Tile communication stuff
  /*migrateRabbits(
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
  );*/

  // Tile local updates
    tileLocalEndOfDayUpdates(tile, currentTimeStep);
}
