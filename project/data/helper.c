Tile *getRandomTile(
  TileGeometry *geometry
) {
  long index = getRandomInt(geometry->ownTileCount);
  return geometry->tiles + (geometry->ownTileIndices[index]);
}

Tile *getRandomImmediatelyAdjacentTile(
  TileGeometry *geometry,
  Tile* tile
) {
  if(tile->adjacency.immediatelyAdjacentTileIndicesCount == 0) {
    return NULL;
  }
  long immediatelyAdjacentTileIndex = getRandomInt(tile->adjacency.immediatelyAdjacentTileIndicesCount);
  long index = tile->adjacency.immediatelyAdjacentTileIndices[immediatelyAdjacentTileIndex];
  return geometry->tiles + index;
}

Tile *getRandomIndirectlyAdjacentTile(
  TileGeometry *geometry,
  Tile* tile
) {
  if(tile->adjacency.indirectlyAdjacentTileIndicesCount == 0) {
    return NULL;
  }
  long indirectlyAdjacentTileIndex = getRandomInt(tile->adjacency.indirectlyAdjacentTileIndicesCount);
  long index = tile->adjacency.indirectlyAdjacentTileIndices[indirectlyAdjacentTileIndex];
  return geometry->tiles + index;
}

Tile *getRandomAdjacentTile(
  TileGeometry *geometry,
  Tile* tile
) {
  long mode = getRandomInt(2);
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

List* getFoxes(
  Tile* tile,
  long timestep
) {
  TileData* data = getTileData(tile, timestep);
  return &data->foxesList;
}