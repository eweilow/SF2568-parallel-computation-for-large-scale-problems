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

  long tileCount = tilesHigh * tilesHigh;
  Tile* tiles = (Tile*)calloc(tileCount, sizeof(Tile));

  for(long i = 0; i < tilesWide; i++) {
    for(long j = 0; j < tilesHigh; j++) {
      bool isLand = (i >= oceanBorderTiles && i < (tilesWide - oceanBorderTiles)) && (j >= oceanBorderTiles && j < (tilesHigh - oceanBorderTiles));

      long tileIndex = i*tilesWide + j;
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

  for(long i = 0; i < tilesWide; i++) {
    for(long j = 0; j < tilesHigh; j++) {
      long self = i*tilesWide + j;
      printf("x = %.2f, y = %.2f\n water: %d\n", tiles[self].x, tiles[self].y, tiles[self].isWaterTile);

      for(long n = 0; n < tiles[self].adjacency.immediatelyAdjacentTileIndicesCount; n++) {
        long other = tiles[self].adjacency.immediatelyAdjacentTileIndices[n];
        printf(" (%.2f, %.2f) -> (%.2f, %.2f)\n", tiles[self].x, tiles[self].y, tiles[other].x, tiles[other].y);
      }
      for(long n = 0; n < tiles[self].adjacency.indirectlyAdjacentTileIndicesCount; n++) {
        long other = tiles[self].adjacency.indirectlyAdjacentTileIndices[n];
        printf(" (%.2f, %.2f) /'''\\> (%.2f, %.2f)\n", tiles[self].x, tiles[self].y, tiles[other].x, tiles[other].y);
      }
    }
  }

  geometry.tileCount = tileCount;
  geometry.tiles = tiles;
  return geometry;
}