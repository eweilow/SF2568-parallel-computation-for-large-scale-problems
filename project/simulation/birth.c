// Birth a rabbit in the provided tile and timestep.
void birthRabbit(Tile* inTile, long timestep) {
  #if SIMULATION_DEBUG
    printf("[SIM.%ld] BIRTH rabbit IN tile.%ld\n", timestep, inTile->id);
  #endif

  Rabbit rabbit;
  rabbit.birthDay = timestep;
  
  // Add the rabbit to the tile list of rabbits
  list_insert(&((inTile->historicalData + timestep)->rabbitsList), &rabbit);
}

// Birth a fox in the provided tile and timestep.
void birthFox(Tile* inTile, long timestep) {
  #if SIMULATION_DEBUG
    printf("[SIM.%ld] BIRTH fox IN tile.%ld\n", timestep, inTile->id);
  #endif

  Fox fox; 

  fox.birthDay = timestep;
  fox.nextMealIndex = 0;

  for(int i = 0; i < MEALS_LENGTH; i++) {
    fox.mealTimeStamps[i] = -1;
  }
  
  // Add the fox to the tile list of foxes
  list_insert(&((inTile->historicalData + timestep)->foxesList), &fox);
}