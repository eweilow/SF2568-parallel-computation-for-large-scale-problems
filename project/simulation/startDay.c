void startDataOfNewDay(Tile* tile, int currentTimeStep) {
  List rabbitsList = tile->historicalData[currentTimeStep - 1].rabbitsList;
  List foxesList = tile->historicalData[currentTimeStep - 1].foxesList;
  double vegetationCount = tile->historicalData[currentTimeStep - 1].vegetation;

  tile->historicalData[currentTimeStep].rabbitsList = list_clone(&rabbitsList);
  tile->historicalData[currentTimeStep].foxesList = list_clone(&foxesList);
  tile->historicalData[currentTimeStep].vegetation = vegetationCount;
}
