/*

*/
void printRabbits(Tile *tile, long ts){
  List *rabbitsList = getRabbits(tile, ts);
  long rabbitsCount;
  Rabbit* rabbits;
  list_read(rabbitsList, &rabbitsCount, (void**)&rabbits);
  for (int i=0; i<rabbitsCount; i++) {
    printf("Rabbit present id: %s birthday: %s\n", rabbits[i].id, rabbits[i].birthDay);
  }
}

void debugSimulateRabbits(Tile *tile, long ts){
  printRabbits(tile, ts);
  bool expectFalse = isRabbitBirthingDay(13);
  printf("isRabbitBirthingDay(13) returned %s - expected False \n", expectFalse);
  bool expectTrue = isRabbitBirthingDay(63);
  printf("isRabbitBirthingDay(63) returned %s - expected True \n", expectTrue);
  rabbitBreeding(tile, ts);
  printRabbits(tile, ts);
}

void debugTileSimulation(TileGeometry* geometry){
  for(long n = 0; n < geometry->tileCount; n++) {
    initializeTile(geometry->tiles + n, TIMESTEPS);
  }
  Tile *tile = geometry->tiles + 0;
  debugSimulateRabbits(tile, 0);
}
