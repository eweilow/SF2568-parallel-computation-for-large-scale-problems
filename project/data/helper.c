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

Tile *getRandomRabbitTile(
  TileGeometry *geometry,
  Tile* tile
) {
  long immediatelyAdjacentTiles = tile->adjacency.immediatelyAdjacentTileIndicesCount;

  long possibleTiles = 1 + immediatelyAdjacentTiles;
  
  long draw = getRandomInt(possibleTiles);
  
  if (draw < 1) {
    return tile;
  } else {
    return getRandomImmediatelyAdjacentTile(geometry, tile);
  }
}

Tile *getRandomFoxTile(
  TileGeometry *geometry,
  Tile* tile
) {
  long immediatelyAdjacentTiles = tile->adjacency.immediatelyAdjacentTileIndicesCount;
  long indirectlyAdjacentTiles = tile->adjacency.immediatelyAdjacentTileIndicesCount;

  long possibleTiles = 1 + immediatelyAdjacentTiles + indirectlyAdjacentTiles;
  
  long draw = getRandomInt(possibleTiles);
  
  if (draw < 1) {
    return tile;
  } else if (draw < 1 + immediatelyAdjacentTiles) { // If immediatelyAdjacentTiles = 0, this will never execute
    return getRandomImmediatelyAdjacentTile(geometry, tile);
  } else {
    return getRandomIndirectlyAdjacentTile(geometry, tile);
  }
}


inline TileData* getTileData(
  Tile* tile,
  long timestep
) {
  return tile->historicalData + timestep;
}

inline List* getRabbits(
  Tile* tile,
  long timestep
) {
  TileData* data = getTileData(tile, timestep);
  return &data->rabbitsList;
}

long getRabbitCount(Tile* tile, long timestep){
  List* rabbits = getRabbits(tile, timestep);
  return lengthOfList(rabbits);
}

inline List* getFoxes(
  Tile* tile,
  long timestep
) {
  TileData* data = getTileData(tile, timestep);
  return &data->foxesList;
}

long getFoxCount(Tile* tile, long timestep){
  List* foxes = getFoxes(tile, timestep);
  return lengthOfList(foxes);
}

double getVegetationLevel(Tile* tile, long timestep){
  TileData* data = getTileData(tile, timestep);
  return data->vegetation;
}
