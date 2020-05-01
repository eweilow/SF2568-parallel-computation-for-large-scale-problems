/*

*/


void debugSimulateRabbits(Tile *tile, long ts){

}

void debugTileSimulation(TileGeometry* geometry){
  for(long n = 0; n < geometry->tileCount; n++) {
    initializeTile(geometry->tiles + n, TIMESTEPS);
  }
  Tile *tile = geometry->tiles + 0;
  debugSimulateRabbits(tile, 0);
}
