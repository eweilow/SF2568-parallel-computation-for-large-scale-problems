// Birth a rabbit in the provided tile and timestep.
bool birthRabbit(Tile* inTile, long timestep) {
  if(inTile->isWaterTile) {
    #if DEBUG_SIMULATION
      printf("[SIM.%ld] SKIPPING RABBIT BIRTH IN WATER TILE tile.%llu.\n", timestep, inTile->id);
    #endif
    return false;
  }

  List *inList = getRabbits(inTile, timestep);

  Rabbit rabbit;
  rabbit.birthDay = timestep;
  rabbit.id = getNextId(inTile->id);

  // Add the rabbit to the tile list of rabbits
  list_insert(inList, &rabbit);

  #if DEBUG_SIMULATION
    if((timestep == 0 && DEBUG_INITIAL_BIRTH) || timestep > 0) {
      char rabbitId[50];
      formatGlobalId(rabbit.id, rabbitId);
      printf("[SIM.%ld] BIRTH rabbit.%s IN tile.%llu\n", timestep, rabbitId, inTile->id);

      #if DEBUG_SIMULATION_DATA
        printf("[SIM.%ld]  >  Created data: ", timestep);
        debugRabbit(&rabbit);
      #endif
    }
  #endif
  return true;
}

// Birth a fox in the provided tile and timestep.
bool birthFox(Tile* inTile, long timestep) {
  if(inTile->isWaterTile) {
    #if DEBUG_SIMULATION
      printf("[SIM.%ld] SKIPPING FOX BIRTH IN WATER TILE tile.%llu.\n", timestep, inTile->id);
    #endif
    return false;
  }

  List *inList = getFoxes(inTile, timestep);

  Fox fox;

  fox.birthDay = timestep;
  fox.id = getNextId(inTile->id);
  fox.hunger = FOXES_MEALS_PER_WEEK_MIN;
  fox.extraMeals = 0.0; //FOXES_MEALS_PER_WEEK_MAX - FOXES_MEALS_PER_WEEK_MIN;
  // Add the fox to the tile list of foxes
  list_insert(inList, &fox);

  #if DEBUG_SIMULATION
    if((timestep == 0 && DEBUG_INITIAL_BIRTH) || timestep > 0) {
      char foxId[50];
      formatGlobalId(fox.id, foxId);
      printf("[SIM.%ld] BIRTH fox.%s IN tile.%llu\n", timestep, foxId, inTile->id);

      #if DEBUG_SIMULATION_DATA
        printf("[SIM.%ld]  >  Created data: ", timestep);
        debugFox(&fox);
      #endif
    }
  #endif
  return true;
}
