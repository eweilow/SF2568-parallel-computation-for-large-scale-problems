Tile *getRandomTile(
  TileGeometry *geometry
) {
  long index = getRandomInt(geometry->tileCount);

  return geometry->tiles + index;
}

Tile *getRandomImmediatelyAdjacentTile(
  TileGeometry *geometry,
  Tile* tile
) {
  long index = getRandomInt(tile->adjacency.indirectlyAdjacentTileIndicesCount);
  return geometry->tiles + index;
}

Tile *getRandomIndirectlyAdjacentTile(
  TileGeometry *geometry,
  Tile* tile
) {
  long index = getRandomInt(tile->adjacency.indirectlyAdjacentTileIndicesCount);
  return geometry->tiles + index;
}

Tile *getRandomAdjacentTile(
  TileGeometry *geometry,
  Tile* tile
) {
  long mode = getRandomInt(1);
  if(mode == 0) {
    return getRandomImmediatelyAdjacentTile(geometry, tile);
  }
  return getRandomIndirectlyAdjacentTile(geometry, tile);
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

long getRabbitCount(Tile* tile, long timestep){
  List* rabbits = getRabbits(tile, timestep);
  size_t* count = (size_t*) -1;
  list_count(rabbits, count);
  long count_long = (long) count;
  return count_long;
}

List* getFoxes(
  Tile* tile,
  long timestep
) {
  TileData* data = getTileData(tile, timestep);
  return &data->foxesList;
}

double getVegetationLevel(Tile* tile,
long timestep){
  TileData* data = getTileData(tile, timestep);
  return data->vegetation;
}
