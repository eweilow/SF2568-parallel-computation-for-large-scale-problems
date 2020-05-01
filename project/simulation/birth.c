long nextId = 0;

// Birth a rabbit in the provided tile and timestep.
void birthRabbit(Tile* inTile, long timestep) {
  List *inList = getRabbits(inTile, timestep);

  Rabbit rabbit;
  rabbit.birthDay = timestep;
  rabbit.id = nextId++;

  // Add the rabbit to the tile list of rabbits
  list_insert(inList, &rabbit);

  #if DEBUG_SIMULATION
    if((timestep == 0 && DEBUG_INITIAL_BIRTH) || timestep > 0) {
      printf("[SIM.%ld] BIRTH rabbit.%d IN tile.%ld\n", timestep, rabbit.id, inTile->id);

      #if DEBUG_SIMULATION_DATA
        printf("[SIM.%ld]  >  Created data: ", timestep);
        debugRabbit(&rabbit);
      #endif
    }
  #endif
}

// Birth a fox in the provided tile and timestep.
void birthFox(Tile* inTile, long timestep) {
  List *inList = getFoxes(inTile, timestep);

  Fox fox;

  fox.birthDay = timestep;
  fox.id = nextId++;
  fox.hunger = 0.7;
  // Add the fox to the tile list of foxes
  list_insert(inList, &fox);

  #if DEBUG_SIMULATION
    if((timestep == 0 && DEBUG_INITIAL_BIRTH) || timestep > 0) {
      printf("[SIM.%ld] BIRTH fox.%d IN tile.%ld\n", timestep, fox.id, inTile->id);

      #if DEBUG_SIMULATION_DATA
        printf("[SIM.%ld]  >  Created data: ", timestep);
        debugFox(&fox);
      #endif
    }
  #endif
}
