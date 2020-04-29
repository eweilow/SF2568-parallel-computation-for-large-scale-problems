// Migrate a rabbit with index 'fromIndex' in tile 'from', from tile 'from' to another tile 'to'.
// This is done in the provided timestep.
bool migrateRabbit(Tile* from, Tile* to, long fromIndex, long timestep) {
  if(from->id == to->id) {
    #if DEBUG_SIMULATION
      printf("[SIM.%ld] SKIPPING RABBIT MIGRATION @ INDEX %ld FROM tile.%llu TO SELF.\n", timestep, fromIndex, from->id);
    #endif
    return true;
  }

  List *fromList = getRabbits(from, timestep);
  List *toList = getRabbits(to, timestep);
  
  long fromListCount = lengthOfList(fromList);
  if(fromListCount == 0) {
    #if DEBUG_SIMULATION
      printf("[SIM.%ld] TILE tile.%llu HAS NO RABBITS. CANNOT MIGRATE @ INDEX %ld TO tile.%ld. \n", timestep, from->id, fromIndex, to->id);
    #endif
    return false;
  }
  if(fromIndex < 0 || fromIndex >= fromListCount) {
    #if DEBUG_SIMULATION
      fprintf(stderr, ANSI_COLOR_RED);
      fprintf(stderr, "[SIM.%ld] CANNOT MIGRATE RABBIT FROM tile.%lllud TO tile.%llu. INVALID INDEX: %ld. EXCEEDS LIST COUNT %ld\n", timestep, from->id, to->id, fromIndex, fromListCount);
      fprintf(stderr, ANSI_COLOR_RESET);
    #endif
    return false;
  }

  // Temporary allocation necessary for the copy
  Rabbit rabbit; 
  
  // Remove the rabbit from the 'from' tile list of rabbits
  list_remove(fromList, fromIndex, &rabbit);
  
  // Add the rabbit to the list of rabbit mgirations
  RabbitMigration migration;
  migration.rabbit = rabbit;
  migration.toTile = to->id;
  list_insert(&from->rabbitMigrationsList, &migration);
  // list_insert(toList, &rabbit);

  #if DEBUG_SIMULATION
    char rabbitId[50];
    formatGlobalId(rabbit.id, rabbitId);
    printf("[SIM.%ld] MIGRATE rabbit.%s @ INDEX %ld OF tile.%llu TO tile.%llu\n", timestep, rabbitId,  fromIndex, from->id, to->id);
    
    #if DEBUG_SIMULATION_DATA
      printf("[SIM.%ld]  >  Moved data: ", timestep);
      debugRabbit(&rabbit);
    #endif
  #endif

  return true;
}

// Migrate a fox with index 'fromIndex' in tile 'from', from tile 'from' to another tile 'to'.
// This is done in the provided timestep.
bool migrateFox(Tile* from, Tile* to, long fromIndex, long timestep) {
  if(from->id == to->id) {
    #if DEBUG_SIMULATION
      printf("[SIM.%ld] SKIPPING FOX MIGRATION @ INDEX %ld FROM tile.%llu TO SELF.\n", timestep, fromIndex, from->id);
    #endif
    return true;
  }

  List *fromList = getFoxes(from, timestep);
  List *toList = getFoxes(to, timestep);
  
  long fromListCount = lengthOfList(fromList);
  if(fromListCount == 0) {
    #if DEBUG_SIMULATION
      printf("[SIM.%ld] TILE tile.%llu HAS NO FOXES. CANNOT MIGRATE @ INDEX %ld TO tile.%llu. \n", timestep, from->id, fromIndex, to->id);
    #endif
    return false;
  }
  if(fromIndex < 0 || fromIndex >= fromListCount) {
    #if DEBUG_SIMULATION
      fprintf(stderr, ANSI_COLOR_RED);
      fprintf(stderr, "[1;31m[SIM.%ld] CANNOT MIGRATE FOX FROM tile.%llu TO tile.%llu. INVALID INDEX: %ld. EXCEEDS LIST COUNT %ld\n", timestep, from->id, to->id, fromIndex, fromListCount);
      fprintf(stderr, ANSI_COLOR_RESET);
    #endif
    return false;
  }

  // Temporary allocation necessary for the copy
  Fox fox; 

  // Remove the fox from the 'from' tile list of foxes
  list_remove(fromList, fromIndex, &fox);
  
  // Add the fox to the list of fox mgirations
  FoxMigration migration;
  migration.fox = fox;
  migration.toTile = to->id;
  list_insert(&from->foxMigrationsList, &migration);
  // list_insert(toList, &fox);

  #if DEBUG_SIMULATION
    char foxId[50];
    formatGlobalId(fox.id, foxId);
    printf("[SIM.%ld] MIGRATE fox.%s @ INDEX %ld OF tile.%llu TO tile.%llu\n", timestep, foxId, fromIndex, from->id, to->id);
    
    #if DEBUG_SIMULATION_DATA
      printf("[SIM.%ld]  >  Moved data: ", timestep);
      debugFox(&fox);
    #endif
  #endif

  return true;
}