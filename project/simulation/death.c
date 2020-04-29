// Birth a rabbit in the provided tile and timestep.
bool killRabbit(Tile* inTile, long index, long timestep) {
  List *inList = getRabbits(inTile, timestep);
  long inListCount = lengthOfList(inList);
  if(inListCount == 0) {
    #if DEBUG_SIMULATION
      printf("[SIM.%ld] TILE tile.%llu HAS NO RABBITS. CANNOT KILL @ %ld\n", timestep, inTile->id, index);
    #endif
    return false;
  }
  if(index < 0 || index >= inListCount) {
    #if DEBUG_SIMULATION
      fprintf(stderr, ANSI_COLOR_RED);
      fprintf(stderr, "[SIM.%ld] CANNOT KILL RABBIT IN tile.%llu. INVALID INDEX: %ld. EXCEEDS LIST COUNT %ld\n", timestep, inTile->id, index, inListCount);
      fprintf(stderr, ANSI_COLOR_RESET);
    #endif
    return false;
  }

  Rabbit rabbit;
  // Remove the rabbit from the list of rabbits
  list_remove(inList, index, &rabbit);

  #if DEBUG_SIMULATION
    char rabbitId[50];
    formatGlobalId(rabbit.id, rabbitId);
    printf("[SIM.%ld] KILL rabbit.%s IN tile.%llu\n", timestep, rabbitId, inTile->id);
    #if DEBUG_SIMULATION_DATA
      printf("[SIM.%ld]  >  Removed data: ", timestep);
      debugRabbit(&rabbit);
    #endif
  #endif
  
  return true;
}

// Birth a fox in the provided tile and timestep.
bool killFox(Tile* inTile, long index,  long timestep) {
  List *inList = getFoxes(inTile, timestep);
  long inListCount = lengthOfList(inList);
  if(inListCount == 0) {
    #if DEBUG_SIMULATION
      printf("[SIM.%ld] TILE tile.%llu HAS NO RABBITS. CANNOT KILL @ %ld\n", timestep, inTile->id, index);
    #endif
    return false;
  }
  if(index < 0 || index >= inListCount) {
    #if DEBUG_SIMULATION
      fprintf(stderr, ANSI_COLOR_RED);
      fprintf(stderr, "[SIM.%ld] CANNOT KILL FOX IN tile.%llu. INVALID INDEX: %ld. EXCEEDS LIST COUNT %ld\n", timestep, inTile->id, index, inListCount);
      fprintf(stderr, ANSI_COLOR_RESET);
    #endif
    return false;
  }

  Fox fox; 
  // Remove the fox from the list of foxes
  list_remove(inList, index, &fox);

  #if DEBUG_SIMULATION
    char foxId[50];
    formatGlobalId(fox.id, foxId);
    printf("[SIM.%ld] KILL fox.%s IN tile.%llu\n", timestep, foxId, inTile->id);
    #if DEBUG_SIMULATION_DATA
      printf("[SIM.%ld]  >  Removed data: ", timestep);
      debugFox(&fox);
    #endif
  #endif
  
  return true;
}