void reduceOutput(Tile *tile, long currentTimeStep) {
  #if USE_REDUCED_OUTPUT
  long foxCount = 0;
  long rabbitCount = 0;
  double totalFoxHunger = 0.0;
  long totalFoxAge = 0;
  long totalRabbitAge = 0;

  long maxFoxAge = 0;
  long maxRabbitAge = 0;

  Rabbit* rabbits;
  list_read(&tile->historicalData[currentTimeStep].rabbitsList, &rabbitCount, (void**)&rabbits);

  Fox* foxes;
  list_read(&tile->historicalData[currentTimeStep].foxesList, &foxCount, (void**)&foxes);

  for(long n = 0; n < rabbitCount; n++) {
    long age = currentTimeStep - rabbits[n].birthDay;
    totalRabbitAge += age;

    if(age > maxRabbitAge) {
      maxRabbitAge = age;
    }
  }

  for(long n = 0; n < foxCount; n++) {
    double hunger = foxes[n].hunger;
    if(hunger < 0.0) {
      hunger = 0.0;
    }
    double extraMeals = foxes[n].extraMeals;
    if(extraMeals < 0.0) {
      extraMeals = 0.0;
    }
    totalFoxHunger += hunger + extraMeals;
    long age = currentTimeStep - foxes[n].birthDay;
    totalFoxAge += age;

    if(age > maxFoxAge) {
      maxFoxAge = age;
    }
  }

  tile->historicalData[currentTimeStep].foxCount = foxCount;
  tile->historicalData[currentTimeStep].rabbitCount = rabbitCount;
  tile->historicalData[currentTimeStep].totalFoxHunger = totalFoxHunger;
  tile->historicalData[currentTimeStep].totalFoxAge = totalFoxAge;
  tile->historicalData[currentTimeStep].totalRabbitAge = totalRabbitAge;
  tile->historicalData[currentTimeStep].maxFoxAge = maxFoxAge;
  tile->historicalData[currentTimeStep].maxRabbitAge = maxRabbitAge;

  // printf("ts = %ld, ", currentTimeStep);
  // printf("foxCount = %ld, ", foxCount);
  // printf("vegetation = %lf, ", tile->historicalData[currentTimeStep].vegetation);
  // printf("rabbitCount = %ld, ", rabbitCount);
  // printf("totalFoxHunger = %lf, ", totalFoxHunger);
  // printf("totalFoxAge = %ld, ", totalFoxAge);
  // printf("totalRabbitAge = %ld\n", totalRabbitAge);
  #endif
}

void startDataOfNewDay(Tile* tile, int currentTimeStep) {
  if(!tile->isOwnedByThisProcess) {
    return;
  }
  
  List rabbitsList = tile->historicalData[currentTimeStep - 1].rabbitsList;
  List foxesList = tile->historicalData[currentTimeStep - 1].foxesList;
  double vegetationCount = tile->historicalData[currentTimeStep - 1].vegetation;

  tile->historicalDataCount = currentTimeStep + 1;

  tile->historicalData[currentTimeStep].rabbitsList = list_clone(&rabbitsList);
  tile->historicalData[currentTimeStep].foxesList = list_clone(&foxesList);
  tile->historicalData[currentTimeStep].vegetation = vegetationCount;

#if USE_REDUCED_OUTPUT
  reduceOutput(tile, (currentTimeStep - 1));
  list_clear(&rabbitsList);
  list_clear(&foxesList);
#endif
}
