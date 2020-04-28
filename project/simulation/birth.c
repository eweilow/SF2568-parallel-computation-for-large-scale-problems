// Birth a rabbit in the provided tile and timestep.
void birthRabbit(Tile* inTile, long timestep) {
  Rabbit rabbit;
  rabbit.birthDay = timestep;
  
  // Add the rabbit to the tile list of rabbits
  list_insert(&((inTile->historicalData + timestep)->rabbitsList), &rabbit);
}

// Birth a fox in the provided tile and timestep.
void birthFox(Tile* inTile, long timestep) {
  Fox fox; 

  fox.birthDay = timestep;
  fox.nextMealIndex = 0;

  for(int i = 0; i < MEALS_LENGTH; i++) {
    fox.mealTimeStamps[i] = -1;
  }
  
  // Add the fox to the tile list of foxes
  list_insert(&((inTile->historicalData + timestep)->foxesList), &fox);
}