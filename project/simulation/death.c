// Birth a rabbit in the provided tile and timestep.
void killRabbit(Tile* inTile, long index, long timestep) {
  Rabbit rabbit;
  // Remove the rabbit from the list of rabbits
  list_remove(&((inTile->historicalData + timestep)->rabbitsList), index, &rabbit);

  #if DEBUG_SIMULATION
    printf("[SIM.%ld] KILL rabbit.%ld IN tile.%ld\n", timestep, rabbit.id, inTile->id);
    #if DEBUG_SIMULATION_DATA
      printf("[SIM.%ld]  >  Removed data: ", timestep);
      debugRabbit(&rabbit);
    #endif
  #endif
}

// Birth a fox in the provided tile and timestep.
void killFox(Tile* inTile, long index,  long timestep) {
  Fox fox; 
  // Remove the fox from the list of foxes
  list_remove(&((inTile->historicalData + timestep)->foxesList), index, &fox);

  #if DEBUG_SIMULATION
    printf("[SIM.%ld] KILL fox.%ld IN tile.%ld\n", timestep, fox.id, inTile->id);
    #if DEBUG_SIMULATION_DATA
      printf("[SIM.%ld]  >  Removed data: ", timestep);
      debugFox(&fox);
    #endif
  #endif
}