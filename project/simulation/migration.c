// Migrate a rabbit with index 'fromIndex' in tile 'from', from tile 'from' to another tile 'to'.
// This is done in the provided timestep.
void migrateRabbit(Tile* from, Tile* to, long fromIndex, long timestep) {
  #if SIMULATION_DEBUG
    printf("[SIM.%ld] MIGRATE rabbit.%ld OF tile.%ld TO tile.%ld\n", timestep, fromIndex, from->id, to->id);
  #endif

  // Temporary allocation necessary for the copy
  Rabbit rabbit; 
  
  // Remove the rabbit from the 'from' tile list of rabbits
  list_remove(&((from->historicalData + timestep)->rabbitsList), fromIndex, &rabbit);
  
  // Add the rabbit to the 'to' tile list of rabbits
  list_insert(&((to->historicalData + timestep)->rabbitsList), &rabbit);
}

// Migrate a fox with index 'fromIndex' in tile 'from', from tile 'from' to another tile 'to'.
// This is done in the provided timestep.
void migrateFox(Tile* from, Tile* to, long fromIndex, long timestep) {
  #if SIMULATION_DEBUG
    printf("[SIM.%ld] MIGRATE fox.%ld OF tile.%ld TO tile.%ld\n", timestep, fromIndex, from->id, to->id);
  #endif

  // Temporary allocation necessary for the copy
  Fox fox; 
  
  // Remove the fox from the 'from' tile list of foxes
  list_remove(&((from->historicalData + timestep)->foxesList), fromIndex, &fox);
  
  // Add the fox to the 'to' tile list of foxes
  list_insert(&((to->historicalData + timestep)->foxesList), &fox);
}