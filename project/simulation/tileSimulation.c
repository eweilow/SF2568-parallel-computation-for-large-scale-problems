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

bool didRabbitDieded(long expLifeSpanDays, long rabbitAge){
  double std = 3;              // How likely rabbits death is to deviate from life expectancy. Try in Matlab for different std.
  double uniformDraw = 0;
  double sigmoidDraw = 0;
  getRandomSigmoidDraw(uniformDraw, sigmoidDraw, expLifeSpanDays - rabbitAge, std);
  return (uniformDraw < sigmoidDraw);
}

void rabbitNaturalDeaths(Tile* tile, long currentTimeStep){
  long vegetationLevel = getVegetationLevel(tile, currentTimeStep);
  long nRabbitsAtStartOfDay = getRabbitCount(tile, currentTimeStep);
  long expLifeSpanDays = rabbitExpectedLifeSpanRule(vegetationLevel);
  List *rabbitsList = getRabbits(tile, currentTimeStep);
  long rabbitsCount;
  Rabbit* rabbits;
  list_read(rabbitsList, &rabbitsCount, (void**)&rabbits);
  for (int i=0; i<nRabbitsAtStartOfDay; i++) {
    //Rabbit *rabbit = list_read();
    long rabbitBirthday = rabbits[i].birthDay;
    long rabbitAge = currentTimeStep - rabbitBirthday;
    if (didRabbitDieded(expLifeSpanDays, rabbitAge)) {
      killRabbit(tile, i, currentTimeStep);
      i--; // list size is reduced??
    }
  }
}

bool didFoxDieded(long expLifeSpanDays, long foxAge){
  double std = 3;              // How likely rabbits death is to deviate from life expectancy. Try in Matlab for different std.
  double uniformDraw = 0;
  double sigmoidDraw = 0;
  getRandomSigmoidDraw(uniformDraw, sigmoidDraw, expLifeSpanDays - foxAge, std);
  return (uniformDraw < sigmoidDraw);
}

void foxNaturalDeaths(Tile* tile, long currentTimeStep){
//  long vegetationLevel = getVegetationLevel(tile, currentTimeStep);
//  long nRabbitsAtStartOfDay = getRabbitCount(tile, currentTimeStep);
//  long nFoxesAtStartOfDay = getFoxCount(tile, currentTimeStep);
  long expLifeSpanDays = foxExpectedLifeSpanRule();
  List *foxesList = getFoxes(tile, currentTimeStep);
  long foxesCount;
  Fox* foxes;
  list_read(foxesList, &foxesCount, (void**)&foxes);
  for (int i=0; i<foxesCount; i++) {
    long foxBirthDay = foxes[i].birthDay;
    long foxAge = currentTimeStep - foxBirthDay;
    if (didFoxDieded(expLifeSpanDays, foxAge)) {
      killFox(tile, i, currentTimeStep);
      i--; // list size is reduced??
    }
  }
}

bool foxHunt(Fox* fox, long foxHuntingSuccessChance){
  double draw = rand();
  bool success = draw < foxHuntingSuccessChance;
  if (success) {
    fox->hunger -= 0.35;
  } else {
    fox->hunger += 0.1;
  }
  return success;
}

void foxHunting(Tile* tile, long currentTimeStep){
  long vegetationLevel = getVegetationLevel(tile, currentTimeStep);
  long nRabbits = getRabbitCount(tile, currentTimeStep);
  List *foxesList = getFoxes(tile, currentTimeStep);
  long foxesCount;
  Fox* foxes;
  list_read(foxesList, &foxesCount, (void**)&foxes);
  for (int i=0; i<foxesCount; i++) {
    if ((foxes + i)->hunger < 0.35) // Fox already full
      continue;

    long foxHuntingSuccessChance = foxHuntingSuccessChanceRule(vegetationLevel, foxesCount, nRabbits);
    bool success = foxHunt(foxes + i, foxHuntingSuccessChance);
    if (success) {
      long rabbitToKill = getRandomInt(nRabbits);
      killRabbit(tile, rabbitToKill, currentTimeStep);
      nRabbits--;
    }
    else if (foxesRiskStarvationRule(foxesCount, nRabbits)){
      long draw = rand();
      if (draw < foxFailedHuntingRiskDeathChanceRule()){
        killFox(tile, i, currentTimeStep);
        i--; // list size is reduced??
      }
    }
  }
}

void killStarvedFoxes(Tile* tile, long currentTimeStep){
  List *foxesList = getFoxes(tile, currentTimeStep);
  long foxesCount;
  Fox* foxes;
  list_read(foxesList, &foxesCount, (void**)&foxes);
  for (int i=0; i<foxesCount; i++) {
    if ((foxes + i)->hunger >= 1.4) {
      killFox(tile, i, currentTimeStep);
      i--; // list size reduced??
    }
  }
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
