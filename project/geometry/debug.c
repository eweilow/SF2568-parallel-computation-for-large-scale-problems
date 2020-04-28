void debugGeometryAdjacency(
  TileGeometry* geometry
){
  Tile* tiles = geometry->tiles;
  for(long i = 0; i < geometry->tilesWide; i++) {
    for(long j = 0; j < geometry->tilesHigh; j++) {
      long self = i*geometry->tilesWide + j;
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
}