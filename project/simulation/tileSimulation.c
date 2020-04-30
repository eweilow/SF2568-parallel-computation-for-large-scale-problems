void vegetationGrowth(Tile* tile, int currentTimeStep) {
  double vegetationStart = getVegetationLevel(tile, currentTimeStep - 1);
  long rabbitCountStart = (long) getRabbitCount(tile, currentTimeStep - 1);
  double vegetationEnd = vegetationAtEndOfDayRule(vegetationStart, rabbitCountStart);
  tile->historicalData[currentTimeStep].vegetation = vegetationEnd;
}

void rabbitBreeding(Tile* tile, long currentTimeStep){
  long vegetationLevel = getVegetationLevel(tile, currentTimeStep);
  long nRabbitsAtStartOfDay = getRabbitCount(tile, currentTimeStep);
  long litterSize = rabbitLitterSizeRule(vegetationLevel, nRabbitsAtStartOfDay);
  long nBirths = nRabbitsAtStartOfDay/2 * litterSize;
  for (int i=0; i<nBirths;i++)
    birthRabbit(tile,currentTimeStep);
}

void foxBreeding(Tile* tile, long currentTimeStep){
  long nFoxesAtStartOfDay = getFoxCount(tile, currentTimeStep);
  long nRabbitsAtStartOfDay = getRabbitCount(tile, currentTimeStep);
  long litterSize = foxLitterSizeRule(nFoxesAtStartOfDay, nRabbitsAtStartOfDay);
  long nBirths = nFoxesAtStartOfDay/2 * litterSize;
  for (int i=0; i<nBirths;i++)
    birthFox(tile,currentTimeStep);
}

void tileLocalStartOfDayUpdates(Tile* tile, long currentTimeStep) {
  startDataOfNewDay(tile, currentTimeStep);
  if (isRabbitBirthingDay(currentTimeStep))
    rabbitBreeding(tile, currentTimeStep);

  if (isFoxBirthingDay(currentTimeStep))
    foxBreeding(tile, currentTimeStep);

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
