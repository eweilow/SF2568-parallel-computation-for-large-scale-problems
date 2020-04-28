// Birth a rabbit in the provided tile and timestep.
void killRabbit(Tile* inTile, long index, long timestep) {
  #if SIMULATION_DEBUG
    printf("[SIM.%ld] KILL rabbit IN tile.%ld\n", timestep, inTile->id);
  #endif

  Rabbit rabbit;
  // Remove the rabbit from the list of rabbits
  list_remove(&((inTile->historicalData + timestep)->rabbitsList), index, &rabbit);
}

// Birth a fox in the provided tile and timestep.
void killFox(Tile* inTile, long index,  long timestep) {
  #if SIMULATION_DEBUG
    printf("[SIM.%ld] KILL fox IN tile.%ld\n", timestep, inTile->id);
  #endif
  Fox fox; 
  // Remove the fox from the list of foxes
  list_remove(&((inTile->historicalData + timestep)->foxesList), index, &fox);
}