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


void debugUpdateTile(
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

  printf("Instantiation done!\n");

  updateTile(
    &(geometry->tiles[0]),
    size1,
    imAdjTiles,
    size2,
    indAdjTiles,
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

  void debugUpdateTiles(
    TileGeometry* geometry,
    long timestep
  ) {

    printf("\ndebug output at timestep %d\n", timestep - 1);
    printf("(%5s, %5s): %8s %8s %8s\n", "x", "y", "rabbits", "foxes", "veg.");
    for(long n = 0; n < geometry->tileCount; n++) {
      printf("(%5.2f, %5.2f): %8d %8d %8.2f\n",
        geometry->tiles[n].x,
        geometry->tiles[n].y,
        geometry->tiles[n].historicalData[timestep-1].rabbitCount,
        geometry->tiles[n].historicalData[timestep-1].foxCount,
        geometry->tiles[n].historicalData[timestep-1].vegetation
      );
    }
    printf("\n ########### updateTiles Start ######## timestep: %d\n", timestep);
    int nTiles = geometry->tileCount;
    for (int j=0; j<nTiles; j++) {
      long *adjTilesIndPtr = geometry->tiles[j].adjacency.immediatelyAdjacentTileIndices;
      long *indAdjTilesIndPtr = geometry->tiles[j].adjacency.indirectlyAdjacentTileIndices;
      int size1 = geometry->tiles[j].adjacency.immediatelyAdjacentTileIndicesCount;
      int size2 = geometry->tiles[j].adjacency.indirectlyAdjacentTileIndicesCount;
      Tile imAdjTiles[size1];
      Tile indAdjTiles[size2];

      for (int i=0; i<size1; i++){
        imAdjTiles[i] = geometry->tiles[adjTilesIndPtr[i]];
      }
      for (int i=0; i<size2; i++){
        indAdjTiles[i] = geometry->tiles[indAdjTilesIndPtr[i]];
      }

      updateTile(
        &(geometry->tiles[j]),
        size1,
        imAdjTiles,
        size2,
        indAdjTiles,
        timestep
      );
    }
    printf("\ndebug output at timestep %d\n", timestep);
    for(long n = 0; n < geometry->tileCount; n++) {
      printf("(%5.2f, %5.2f): %8d %8d %8.2f\n",
        geometry->tiles[n].x,
        geometry->tiles[n].y,
        geometry->tiles[n].historicalData[timestep].rabbitCount,
        geometry->tiles[n].historicalData[timestep].foxCount,
        geometry->tiles[n].historicalData[timestep].vegetation
      );
    }
    printf("\ndebug output of previous timestep at timestep %d\n", timestep);
    for(long n = 0; n < geometry->tileCount; n++) {
      printf("(%5.2f, %5.2f): %8d %8d %8.2f\n",
        geometry->tiles[n].x,
        geometry->tiles[n].y,
        geometry->tiles[n].historicalData[timestep-1].rabbitCount,
        geometry->tiles[n].historicalData[timestep-1].foxCount,
        geometry->tiles[n].historicalData[timestep-1].vegetation
      );
    }
}
