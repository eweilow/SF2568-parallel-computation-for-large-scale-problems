void debugTiles(
  TileGeometry* geometry,
  long timestep
) {

  printf("\ndebug output at timestep %d\n", timestep);
  printf("(%5s, %5s): %8s %8s %8s\n", "x", "y", "rabbits", "foxes", "veg.");
  for(long n = 0; n < geometry->tileCount; n++) {
    printf("(%5.2f, %5.2f): %8d %8d %8.2f\n",
      geometry->tiles[n].x,
      geometry->tiles[n].y,
      geometry->tiles[n].historicalData[timestep].rabbitCount,
      geometry->tiles[n].historicalData[timestep].foxCount,
      geometry->tiles[n].historicalData[timestep].vegetation
    );
  }
}


void debugUpdateTiles(
  TileGeometry* geometry,
  long timestep
) {

  printf("\ndebug output at timestep %d\n", timestep);
  printf("(%5s, %5s): %8s %8s %8s\n", "x", "y", "rabbits", "foxes", "veg.");
  for(long n = 0; n < geometry->tileCount; n++) {
    printf("(%5.2f, %5.2f): %8d %8d %8.2f\n",
      geometry->tiles[n].x,
      geometry->tiles[n].y,
      geometry->tiles[n].historicalData[timestep].rabbitCount,
      geometry->tiles[n].historicalData[timestep].foxCount,
      geometry->tiles[n].historicalData[timestep].vegetation
    );
  }
  printf("\n ########### updateTile Start ######## timestep: %d\n", timestep);

  long *adjTilesIndPtr = geometry->tiles[0].adjacency.immediatelyAdjacentTileIndices;
  long *indAdjTilesIndPtr = geometry->tiles[0].adjacency.indirectlyAdjacentTileIndices;
  int size1 = geometry->tiles[0].adjacency.immediatelyAdjacentTileIndicesCount;
  int size2 = geometry->tiles[0].adjacency.indirectlyAdjacentTileIndicesCount;
  Tile imAdjTiles[size1];
  Tile indAdjTiles[size2];

  for (int i=0; i<size1; i++){
    imAdjTiles[i] = geometry->tiles[adjTilesIndPtr[i]];
  }
  for (int i=0; i<size2; i++){
    indAdjTiles[i] = geometry->tiles[indAdjTilesIndPtr[i]];
  }
  Tile *imAdjTilesPtr;
  Tile *indAdjTilesPtr;

  printf("\n Instantiation done!");

  updateTile(
    &geometry->tiles[0],
    geometry->tiles[0].adjacency.immediatelyAdjacentTileIndicesCount,
    imAdjTilesPtr,
    geometry->tiles[0].adjacency.indirectlyAdjacentTileIndicesCount,
    indAdjTilesPtr,
    timestep
  );
  printf("\ndebug output at timestep %d\n", timestep+1);
  for(long n = 0; n < geometry->tileCount; n++) {
    printf("(%5.2f, %5.2f): %8d %8d %8.2f\n",
      geometry->tiles[n].x,
      geometry->tiles[n].y,
      geometry->tiles[n].historicalData[timestep+1].rabbitCount,
      geometry->tiles[n].historicalData[timestep+1].foxCount,
      geometry->tiles[n].historicalData[timestep+1].vegetation
    );
  }
}
