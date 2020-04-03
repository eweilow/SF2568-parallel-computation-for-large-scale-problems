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