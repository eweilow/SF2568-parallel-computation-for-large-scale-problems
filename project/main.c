#include <stdbool.h>
#include <stddef.h>

#include "./types.c"
#include "./simulation.c"
#include "./geometry.c"
#include "./data.c"

void main(int argc, char **argv)
{
  spawnMPI();

  // initialize geometry data on root process
  TileGeometry geometry = generateIsland(4, 4, 0.2, 0.0);

  // initialize initial element of historicalData

  // PARALLEL ONLY: scatter tiles and adjacency from root to children

  // PARALLEL ONLY: create map between global and local indices
  // PARALLEL ONLY: create map between local and global indices

  // PARALLEL ONLY: create map for data exchanging

  // initialize future historicalData in each tile on every child
  
  // for each step {
    // run simulation on tiles local to this tile

    // PARALLEL ONLY: exchange data as necessary, according to the exchange map
  // }

  // store to file in each child

  murderMPI();
}