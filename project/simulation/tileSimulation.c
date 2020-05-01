void vegetationGrowth(Tile* tile, int currentTimeStep) {
  double vegetationStart = getVegetationLevel(tile, currentTimeStep - 1);
  long rabbitCountStart = (long) getRabbitCount(tile, currentTimeStep - 1);
  double vegetationEnd = vegetationAtEndOfDayRule(vegetationStart, rabbitCountStart);
  tile->historicalData[currentTimeStep].vegetation = vegetationEnd;
}

void rabbitBreeding(Tile* tile, long currentTimeStep){
  #if DEBUG_STEPS
  if(tile->id == 0) {
    printf("Breeding rabbits in day %ld\n", currentTimeStep);
  }
  #endif
  long vegetationLevel = getVegetationLevel(tile, currentTimeStep);
  long nRabbitsAtStartOfDay = getRabbitCount(tile, currentTimeStep);
  long litterSize = rabbitLitterSizeRule(vegetationLevel, nRabbitsAtStartOfDay);
  long nBirths = nRabbitsAtStartOfDay/2 * litterSize;

  for (int i=0; i<nBirths;i++)
    birthRabbit(tile,currentTimeStep);
}

void foxBreeding(Tile* tile, long currentTimeStep){
  #if DEBUG_STEPS
  if(tile->id == 0) {
    printf("Breeding foxes in day %ld\n", currentTimeStep);
  }
  #endif
  long nFoxesAtStartOfDay = getFoxCount(tile, currentTimeStep);
  long nRabbitsAtStartOfDay = getRabbitCount(tile, currentTimeStep);
  long litterSize = foxLitterSizeRule(nFoxesAtStartOfDay, nRabbitsAtStartOfDay);
  long nBirths = nFoxesAtStartOfDay/2 * litterSize;
  for (int i=0; i<nBirths;i++)
    birthFox(tile,currentTimeStep);
}

bool didRabbitDieded(long expLifeSpanDays, long rabbitAge){
  const double std = 1.0; // How likely rabbits death is to deviate from life expectancy. Try in Matlab for different std.
  double uniformDraw = getRandomDouble();
  double probabilityOfDeath = getSigmoidProbability((double)rabbitAge, (double)expLifeSpanDays, std);
  return uniformDraw <= probabilityOfDeath;
}

void rabbitNaturalDeaths(Tile* tile, long currentTimeStep){
  long vegetationLevel = getVegetationLevel(tile, currentTimeStep);
  long expLifeSpanDays = rabbitExpectedLifeSpanRule(vegetationLevel);
  
  List *rabbitsList = getRabbits(tile, currentTimeStep);

  long rabbitsCount;
  Rabbit* rabbits;
  list_read(rabbitsList, &rabbitsCount, (void**)&rabbits);

  for (int i=0; i < rabbitsCount; i++) {
    //Rabbit *rabbit = list_read();
    long rabbitBirthday = rabbits[i].birthDay;
    long rabbitAge = currentTimeStep - rabbitBirthday;
    if (didRabbitDieded(expLifeSpanDays, rabbitAge)) {
      killRabbit(tile, i, currentTimeStep);
      i--; // list size is reduced??
      rabbitsCount--;
    }
  }
}

bool didFoxDieded(long expLifeSpanDays, long foxAge){
  const double std = 1.0; // How likely rabbits death is to deviate from life expectancy. Try in Matlab for different std.
  double uniformDraw = getRandomDouble();
  double probabilityOfDeath = getSigmoidProbability((double)foxAge, (double)expLifeSpanDays, std);
  return uniformDraw <= probabilityOfDeath;
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
  for (int i=0; i < foxesCount; i++) {
    long foxBirthDay = foxes[i].birthDay;
    long foxAge = currentTimeStep - foxBirthDay;
    if (didFoxDieded(expLifeSpanDays, foxAge)) {
      killFox(tile, i, currentTimeStep);
      i--; // list size is reduced??
      foxesCount--;
    }
  }
}

bool foxHunt(Fox* fox, double foxHuntingSuccessChance, long nRabbits){
  double draw = getRandomDouble();
  bool success = nRabbits > 0 && draw < foxHuntingSuccessChance;

  if(fox->hunger < 0.0) {
    printf("fox->hunger < 0.0\n");
    exit(1);
  }
  if(fox->hunger > 2.0) {
    printf("fox->hunger > 2.0\n");
    exit(1);
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
    bool isFull = (foxes + i)->hunger < 0.35;
    if (!isFull) {
      double foxHuntingSuccessChance = foxHuntingSuccessChanceRule(vegetationLevel, foxesCount, nRabbits);
      bool success = foxHunt(foxes + i, foxHuntingSuccessChance, nRabbits);
      if (success) {
        //printf("succesful hunt! %d\n", success);
        long rabbitToKill = getRandomInt(nRabbits);
        killRabbit(tile, rabbitToKill, currentTimeStep);
        nRabbits--;
        (foxes + i)->hunger -= 0.35;
      }
      else if (foxesRiskStarvationRule(foxesCount, nRabbits)){
        double draw = getRandomDouble();
        if (draw < foxFailedHuntingRiskDeathChanceRule()){
          killFox(tile, i, currentTimeStep);
          i--; // list size is reduced??
          foxesCount--;
        } else {
          (foxes + i)->hunger += 0.1;
        }
      }
    } else {
      (foxes + i)->hunger += 0.1;
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
      foxesCount--;
    }
  }
}

void tileLocalStartOfDayUpdates(Tile* tile, long currentTimeStep) {
  if (isRabbitBirthingDay(currentTimeStep))
    rabbitBreeding(tile, currentTimeStep);

  if (isFoxBirthingDay(currentTimeStep))
    foxBreeding(tile, currentTimeStep);

}

void tileLocalMiddleOfDayUpdates(Tile* tile, long currentTimeStep) {
  rabbitNaturalDeaths(tile, currentTimeStep);
  foxNaturalDeaths(tile, currentTimeStep);
  foxHunting(tile, currentTimeStep);
  killStarvedFoxes(tile, currentTimeStep);
}

void tileLocalEndOfDayUpdates(Tile* tile, long currentTimeStep) {
  vegetationGrowth(tile, currentTimeStep);
}
