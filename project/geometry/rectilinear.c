void generateAdjacencyMap(
  long n,
  long i,
  long j,
  long tilesWide, 
  long tilesHigh,
  long* count,
  long** indices
) {
  long _i, _j, _index;
  for(long o = -n; o <= n; o++) {
    _i = i + o;
    _j = j - n;
    
    if(_i >= 0 && _i < tilesWide && _j >= 0 && _j < tilesHigh) {
      _index = _i*tilesWide + _j;
      (*indices)[
        (*count)++
      ] = _index;
    }

    _i = i + o;
    _j = j + n;

    if(_i >= 0 && _i < tilesWide && _j >= 0 && _j < tilesHigh) {
      _index = _i*tilesWide + _j;
      (*indices)[
        (*count)++
      ] = _index;
    }
  }

  for(long o = -n+1; o <= n-1; o++) {
    _i = i - n;
    _j = j + o;
    
    if(_i >= 0 && _i < tilesWide && _j >= 0 && _j < tilesHigh) {
      _index = _i*tilesWide + _j;
      (*indices)[
        (*count)++
      ] = _index;
    }

    _i = i + n;
    _j = j + o;

    if(_i >= 0 && _i < tilesWide && _j >= 0 && _j < tilesHigh) {
      _index = _i*tilesWide + _j;
      (*indices)[
        (*count)++
      ] = _index;
    }
  }
}

// generate the island
TileGeometry generateIsland(
  long tilesWide, 
  long tilesHigh, 
  long oceanBorderTiles,
  double randomWaterFactor
) {
  TileGeometry geometry;
  geometry.tilesWide = tilesWide;
  geometry.tilesHigh = tilesHigh;

  long tileCount = tilesWide * tilesHigh;
  Tile* tiles = (Tile*)calloc(tileCount, sizeof(Tile));

  for(long i = 0; i < tilesWide; i++) {
    for(long j = 0; j < tilesHigh; j++) {
      bool isLand = (i >= oceanBorderTiles && i < (tilesWide - oceanBorderTiles)) && (j >= oceanBorderTiles && j < (tilesHigh - oceanBorderTiles));

      u_int64_t tileIndex = (u_int64_t)(i*tilesHigh + j);
      tiles[tileIndex].id = tileIndex;
      tiles[tileIndex].x = (double) i;
      tiles[tileIndex].y = (double) j;
      tiles[tileIndex].isWaterTile = !isLand;
      tiles[tileIndex].historicalDataCount = 0;
      tiles[tileIndex].historicalData = (TileData*) NULL;

      tiles[tileIndex].adjacency.immediatelyAdjacentTileIndicesCount = 0;
      tiles[tileIndex].adjacency.indirectlyAdjacentTileIndicesCount = 0;
      tiles[tileIndex].adjacency.immediatelyAdjacentTileIndices = (long*)calloc(8, sizeof(long));
      tiles[tileIndex].adjacency.indirectlyAdjacentTileIndices = (long*)calloc(16, sizeof(long));

      generateAdjacencyMap(1, i, j, tilesWide, tilesWide, &tiles[tileIndex].adjacency.immediatelyAdjacentTileIndicesCount, &tiles[tileIndex].adjacency.immediatelyAdjacentTileIndices);
      generateAdjacencyMap(2, i, j, tilesWide, tilesWide, &tiles[tileIndex].adjacency.indirectlyAdjacentTileIndicesCount, &tiles[tileIndex].adjacency.indirectlyAdjacentTileIndices);
    }
  }

  geometry.tileCount = tileCount;
  geometry.tiles = tiles;
  return geometry;
}