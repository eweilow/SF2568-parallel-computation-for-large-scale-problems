/*

*/
void printRabbits(Tile *tile, long ts){
  List *rabbitsList = getRabbits(tile, ts);
  long rabbitsCount;
  Rabbit* rabbits;
  list_read(rabbitsList, &rabbitsCount, (void**)&rabbits);
  for (int i=0; i<rabbitsCount; i++) {
    printf("Rabbit present id: %ld birthday: %ld\n", rabbits[i].id, rabbits[i].birthDay);
  }
}

void printNumberOfRabbits(Tile* tile, long ts) {
  long nRabbits = getRabbitCount(tile, ts);
  printf("Number of rabbits: %ld at timestep: %ld \n", nRabbits, ts);
}

void printFoxes(Tile *tile, long ts){
  List *foxesList = getFoxes(tile, ts);
  long foxesCount;
  Fox* foxes;
  list_read(foxesList, &foxesCount, (void**)&foxes);
  for (int i=0; i<foxesCount; i++) {
    printf("Fox present id: %ld birthday: %ld hunger: %f \n", foxes[i].id, foxes[i].birthDay, foxes[i].hunger);
  }
}

void printNumberOfFoxes(Tile* tile, long ts) {
  long nFoxes = getFoxCount(tile, ts);
  printf("Number of foxes: %ld at timestep: %ld \n", nFoxes, ts);
}

void debugSimulateBreRabbitsBreeding(Tile *tile, long ts){
  printf("********* DEBUG SIMULATE RABBITS Breeding START ************* %ld\n", ts);
  printf("Rabbits before breeding \n");
  printRabbits(tile, ts);

  bool expectFalse = isRabbitBirthingDay(13);
  printf("isRabbitBirthingDay(13) returned %d - expected 0 \n", expectFalse);
  bool expectTrue = isRabbitBirthingDay(63);
  printf("isRabbitBirthingDay(63) returned %d - expected 1 \n", expectTrue);

  rabbitBreeding(tile, ts);
  printf("Rabbits after breeding \n");
  printRabbits(tile, ts);

  printf("********* DEBUG SIMULATE RABBITS Breeding END ************* %ld\n", ts);

}

void debugSimulateFoxesBreeding(Tile *tile, long ts){
  printf("********* DEBUG SIMULATE Foxes Breeding START ************* %ld\n", ts);
  printf("Foxes before breeding \n");
  printNumberOfFoxes(tile, ts);

  bool expectFalse = isFoxBirthingDay(13);
  printf("isFoxBirthingDay(13) returned %d - expected 0 \n", expectFalse);
  bool expectTrue = isFoxBirthingDay(12*28);
  printf("isFoxBirthingDay(12*28) returned %d - expected 1 \n", expectTrue);

  foxBreeding(tile, ts);
  printf("Foxes after breeding \n");
  printNumberOfFoxes(tile, ts);
  printf("********* DEBUG SIMULATE Foxes Breeding END ************* %ld\n", ts);
}

void debugSimulateFoxesHunting(Tile *tile, long ts){
  printf("********* DEBUG SIMULATE Foxes Hunting START ************* %ld\n", ts);
  printf("BEFORE hunting \n");
  printFoxes(tile, ts);
  printNumberOfFoxes(tile, ts);
  printNumberOfRabbits(tile, ts);
  foxHunting(tile, ts);
  printf("AFTER hunting \n");
  printFoxes(tile, ts);
  printNumberOfRabbits(tile, ts);
  printNumberOfFoxes(tile, ts);

  printf("********* DEBUG SIMULATE RABBITS Hunting END ************* %ld\n", ts);
}

void debugTileSimulation(TileGeometry* geometry){
  for(long n = 0; n < geometry->ownTileCount; n++) {
    long i = geometry->ownTileIndices[n];
    initializeTile(geometry->tiles + i, TIMESTEPS);
  }

  long firstLocalTile = geometry->ownTileIndices[0];

  Tile *tile = geometry->tiles + firstLocalTile;
  debugSimulateBreRabbitsBreeding(tile, 0);
  debugSimulateFoxesBreeding(tile, 0);
  debugSimulateFoxesHunting(tile, 0);
}
