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
      _index = _i*tilesHigh + _j;
      (*indices)[
        (*count)++
      ] = _index;
    }

    _i = i + o;
    _j = j + n;

    if(_i >= 0 && _i < tilesWide && _j >= 0 && _j < tilesHigh) {
      _index = _i*tilesHigh + _j;
      (*indices)[
        (*count)++
      ] = _index;
    }
  }

  for(long o = -n+1; o <= n-1; o++) {
    _i = i - n;
    _j = j + o;
    
    if(_i >= 0 && _i < tilesWide && _j >= 0 && _j < tilesHigh) {
      _index = _i*tilesHigh + _j;
      (*indices)[
        (*count)++
      ] = _index;
    }

    _i = i + n;
    _j = j + o;

    if(_i >= 0 && _i < tilesWide && _j >= 0 && _j < tilesHigh) {
      _index = _i*tilesHigh + _j;
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
  double randomWaterFactor,
  long processesWide,
  long processesHigh,
  long rank
) {
  const double tileSize = 1.0;

  double w = ((double) tilesWide + 1.0) * tileSize;
  double h = ((double) tilesHigh + 1.0) * tileSize;

  double dpx = w / (double)processesWide;
  double dpy = h / (double)processesHigh;

  if(dpx < 2.0*tileSize) {
    printf("PANIC: dpx < 2.0*tileSize\n");
    printf(" This likely means that there are too few tiles wide\n");
    exit(1);
  }
  if(dpy < 2.0*tileSize) {
    printf("PANIC: dpy < 2.0*tileSize\n");
    printf(" This likely means that there are too few tiles high\n");
    exit(1);
  }

  TileGeometry geometry;
  geometry.tilesWide = tilesWide;
  geometry.tilesHigh = tilesHigh;

  long tileCount = tilesWide * tilesHigh;
  Tile* tiles = (Tile*)calloc(tileCount, sizeof(Tile));

  long ownTileCount = 0;
  for(long i = 0; i < tilesWide; i++) {
    for(long j = 0; j < tilesHigh; j++) {
      long distanceToClosestBorder = i;
      if(j < distanceToClosestBorder) {
        distanceToClosestBorder = j;
      }
      if(tilesWide - (i+1) < distanceToClosestBorder) {
        distanceToClosestBorder = tilesWide - (i+1);
      }
      if(tilesHigh - (j+1) < distanceToClosestBorder) {
        distanceToClosestBorder = tilesWide - (j+1);
      }

      double distanceToBorder = (double)distanceToClosestBorder + tileSize/2;

      double isWaterProbability = exp(-distanceToBorder) * (1.0 - randomWaterFactor) + randomWaterFactor;
      if(isWaterProbability > 1.0) {
        isWaterProbability = 1.0;
      }
      if(distanceToBorder < oceanBorderTiles) {
        isWaterProbability = 1.0;
      }

      bool isWaterTile = isWaterProbability >= 1.0 ? 1 : (getRandomDouble() <= isWaterProbability);


      double x = ((double) (i + 1)) * tileSize;
      double y = ((double) (j + 1)) * tileSize;
      // printf("%lf %lf: %lf %d\n", x, y, isWaterProbability, isWaterTile);

      long px = (long)floor((x - tileSize/2) / dpx);
      long py = (long)floor((y - tileSize/2) / dpy);

      long process = px * processesHigh + py;

      u_int64_t tileId = (u_int64_t)(i*tilesHigh + j);
      // printf("%llu: %lf %lf: %ld %ld = %ld\n", tileIndex, x, y, px, py, process);

      long tileIndex = (long) tileId;
      tiles[tileIndex].id = tileId;
      tiles[tileIndex].process = process;
      tiles[tileIndex].isOwnedByThisProcess = rank == process;

      tiles[tileIndex].x = x;
      tiles[tileIndex].y = y;
      tiles[tileIndex].isWaterTile = isWaterTile;
      tiles[tileIndex].historicalDataCount = 0;
      tiles[tileIndex].historicalData = (TileData*) NULL;

      tiles[tileIndex].adjacency.immediatelyAdjacentTileIndicesCount = 0;
      tiles[tileIndex].adjacency.indirectlyAdjacentTileIndicesCount = 0;
      tiles[tileIndex].adjacency.immediatelyAdjacentTileIndices = (long*)calloc(8, sizeof(long));
      tiles[tileIndex].adjacency.indirectlyAdjacentTileIndices = (long*)calloc(16, sizeof(long));

      // Unless this is true, we will never need this data
      if(tiles[tileIndex].isOwnedByThisProcess) {
        ++ownTileCount;
        if(!isWaterTile) {
          generateAdjacencyMap(1, i, j, tilesWide, tilesHigh, &tiles[tileIndex].adjacency.immediatelyAdjacentTileIndicesCount, &tiles[tileIndex].adjacency.immediatelyAdjacentTileIndices);
          generateAdjacencyMap(2, i, j, tilesWide, tilesHigh, &tiles[tileIndex].adjacency.indirectlyAdjacentTileIndicesCount, &tiles[tileIndex].adjacency.indirectlyAdjacentTileIndices);
        }
      }
    }
  }

  geometry.tileCount = tileCount;
  geometry.tiles = tiles;
  
  geometry.ownTileIndices = (long*)malloc(ownTileCount * sizeof(long));
  long ptr = 0;
  for(long n = 0; n < geometry.tileCount; n++) {
    if(tiles[n].isOwnedByThisProcess) {
      geometry.ownTileIndices[ptr++] = n;
    }
  }

  return geometry;
}