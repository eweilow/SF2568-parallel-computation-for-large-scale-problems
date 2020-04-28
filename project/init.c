static int MAX_TILE_FOX_COUNT = 40;

Rabbit initializeRabbit(long timestep){
  Rabbit rabbit;
  rabbit.birthDay = timestep;
  return rabbit;
}

Rabbit* initializeRabbits(long nRabbits, long timestep){
  Rabbit* rabbits = (Rabbit*) calloc(nRabbits, sizeof(Rabbit));
  for(int i=0; i < nRabbits; i++) {
    rabbits[i] = initializeRabbit(timestep);
  }
  return rabbits;
}

Fox initializeFox(long timestep) {
  Fox fox;
  fox.birthDay = timestep;
  fox.mealTimeStamps = (long*) calloc(4, sizeof(long));
  long emptyStomach[4] = {-1, -1, -1, -1};
  fox.mealTimeStamps = emptyStomach;
  return fox;
}

Fox* initializeFoxes(long nFoxes, long timestep) {
  Fox* foxes = (Fox*) calloc(MAX_TILE_FOX_COUNT, sizeof(Fox));
  for(int i=0; i < nFoxes; i++) {
    foxes[i] = initializeFox(timestep);
  }
  return foxes;
}

void initializeTile(
  Tile* tile,
  long timesteps
) {
  tile->historicalData = (TileData*) calloc(timesteps, sizeof(TileData));

  TileData initialData;

  initialData.rabbitCount = 13;
  initialData.rabbits = initializeRabbits(13, 0);
  initialData.foxCount = 7;
  initialData.foxes = initializeFoxes(7, 0);
  initialData.vegetation = 0.25;

  tile->historicalData[0] = initialData;
}
