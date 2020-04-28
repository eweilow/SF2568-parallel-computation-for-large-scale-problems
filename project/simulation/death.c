// Birth a rabbit in the provided tile and timestep.
void killRabbit(Tile* inTile, long index, long timestep) {
  Rabbit rabbit;
  // Remove the rabbit from the list of rabbits
  list_remove(&((inTile->historicalData + timestep)->rabbitsList), index, &rabbit);
}

// Birth a fox in the provided tile and timestep.
void killFox(Tile* inTile, long index,  long timestep) {
  Fox fox; 
  // Remove the fox from the list of foxes
  list_remove(&((inTile->historicalData + timestep)->foxesList), index, &fox);
}