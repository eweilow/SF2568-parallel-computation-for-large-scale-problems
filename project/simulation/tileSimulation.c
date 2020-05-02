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

void foxHunting(Tile* tile, long currentTimeStep){
  long vegetationLevel = getVegetationLevel(tile, currentTimeStep);
  long nRabbits = getRabbitCount(tile, currentTimeStep);
  long nStartRabbits = nRabbits;

  List *foxesList = getFoxes(tile, currentTimeStep);
  long foxesCount;
  Fox* foxes;
  list_read(foxesList, &foxesCount, (void**)&foxes);

  for (int i=0; i<foxesCount; i++) {
    double totalHunger = (foxes + i)->hunger + (foxes + i)->extraMeals;

    bool canEat = false;
    bool isEatingExtra = false;

    long sustainablePopulation = foxesCount * 2 * 9; // The book says something about sustainable population
    bool isUnsustainable = nRabbits < sustainablePopulation;

    if(nRabbits > 0) {
      canEat = totalHunger <= FOXES_MEALS_PER_WEEK_MAX - 1.0;
      isEatingExtra = vegetationLevel < 0.6 && totalHunger >= FOXES_MEALS_PER_WEEK_MIN;
    }

    bool didSnacc = false;
    if (canEat) {
      double chanceOfSuccess = 1.0;

      if(isUnsustainable) {
        // if there are not a sustainable population, assume that foxes have a harder time rabbits
        chanceOfSuccess *= nRabbits / (double)sustainablePopulation;
      }

      if(isEatingExtra) {
        // if foxes are eating extra, I.E they can survive but are eating more for leisure, assume they are less effective at hunting
        chanceOfSuccess *= 0.5;
      }

      double draw = getRandomDouble();
      bool success = draw < chanceOfSuccess;
      if (success) {
        // If fox had a successful hunt...
        long rabbitToKill = getRandomInt(nRabbits);
        killRabbit(tile, rabbitToKill, currentTimeStep);
        nRabbits--;

        (foxes + i)->hunger += 1.0;

        // Add excess hunger to the extra meals counter
        if((foxes + i)->hunger > FOXES_MEALS_PER_WEEK_MIN) {
          double addToExtraMeals = (foxes + i)->hunger - FOXES_MEALS_PER_WEEK_MIN;
          if(addToExtraMeals > 0.0) {
            (foxes + i)->extraMeals += addToExtraMeals;
            (foxes + i)->hunger = FOXES_MEALS_PER_WEEK_MIN;
          }
        }
        // truncate the extra meals counter
        if((foxes + i)->extraMeals > FOXES_EXTRA_MEALS_PER_WEEK) {
          (foxes + i)->extraMeals = FOXES_EXTRA_MEALS_PER_WEEK;
        }

        didSnacc = true;
      }
    }
    
    if(!didSnacc) {
      // foxes that did not eat get hungrier each day
      (foxes + i)->extraMeals -= FOXES_DECREASE_PER_DAY;
      (foxes + i)->hunger -= FOXES_DECREASE_PER_DAY;
    }

    if((foxes + i)->extraMeals < 0.0) {
      (foxes + i)->extraMeals = 0.0;
    }
  }
}

void killStarvedFoxes(Tile* tile, long currentTimeStep){
  List *foxesList = getFoxes(tile, currentTimeStep);
  long foxesCount;
  Fox* foxes;
  list_read(foxesList, &foxesCount, (void**)&foxes);
  for (int i=0; i<foxesCount; i++) {
    if ((foxes + i)->hunger <= 0) {
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
