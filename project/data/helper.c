Tile *getRandomTile(
  TileGeometry *geometry
) {
  long index = getRandomInt(geometry->tileCount);

  return geometry->tiles + index;
}

TileData* getTileData(
  Tile* tile,
  long timestep
) {
  return tile->historicalData + timestep;
}

List* getRabbits(
  Tile* tile,
  long timestep
) {
  TileData* data = getTileData(tile, timestep);
  return &data->rabbitsList;
}

List* getFoxes(
  Tile* tile,
  long timestep
) {
  TileData* data = getTileData(tile, timestep);
  return &data->foxesList;
}