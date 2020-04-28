#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define TIMESTEPS 1000

#define TEST_LIST 0
#define DEBUG_LIST 0
#define DEBUG_GEOMETRY 0
#define DEBUG_INITIAL_BIRTH 0 // Print debug output in birthRabbit/birthFox in timestep 0
#define DEBUG_SIMULATION 1
#define DEBUG_SIMULATION_DATA 0


#include "./utils/rand.c"
#include "./utils/list.c"

#include "./data/mpi.c"
#include "./data/types.c"

#include "./debug.c"

#include "./simulation/migration.c"
#include "./simulation/birth.c"
#include "./simulation/death.c"

#include "./simulation.c"
#include "./geometry/rectilinear.c"
#include "./geometry/debug.c"

#include "./init.c"


#if TEST_LIST

void main(int argc, char **argv)
{
  List list = initList(sizeof(Fox), 0);
  List list2 = list_clone(&list);

  Fox fox;
  list_insert(&list2, &fox);
  list_remove(&list2, 0, &fox);
  
  list = initList(sizeof(double), 0);

  for(double d = 0.0; d < 100.0; d += 1) {
    list_insert(&list, &d);
  }

  long count;
  double* data;

  double removed;
  for(long l = 0; l < 10; l++) {
    list_count(&list, &count);
    list_remove(&list, getRandomInt(count), &removed);
  }

  list_read(&list, &count, (void**)&data);
  printf("%ld: %lf\n", count-1, data[count-1]);

  for(long l = 0; l < 10; l++) {
    list_count(&list, &count);
    list_remove(&list, getRandomInt(count), &removed);
  }
  
  list_read(&list, &count, (void**)&data);
  printf("%ld: %lf\n", count-1, data[count-1]);
}

#else

void main(int argc, char **argv)
{
  spawnMPI();

  // initialize geometry data on root process
  TileGeometry geometry = generateIsland(2, 2, 0, 0.0);

  #if DEBUG_GEOMETRY
    debugGeometryAdjacency(&geometry);
  #endif

  for(long n = 0; n < geometry.tileCount; n++) {
    initializeTile(geometry.tiles + n, TIMESTEPS);
  }
  // migrateFox(geometry.tiles + 0, geometry.tiles + 1, 0, 0);

  debugTiles(&geometry, 0);
  for(long n = 0; n < geometry.tileCount; n++) {
    startDataOfNewDay(geometry.tiles + n, 1);
  }

  migrateFox(geometry.tiles + 0, geometry.tiles + 1, 0, 1);
  birthRabbit(geometry.tiles + 2, 1);
  birthRabbit(geometry.tiles + 3, 1);
  killRabbit(geometry.tiles + 3, 0, 1);
  birthFox(geometry.tiles + 1, 1);
  birthFox(geometry.tiles + 1, 1);
  birthFox(geometry.tiles + 1, 1);
  birthFox(geometry.tiles + 1, 1);
  migrateFox(geometry.tiles + 1, geometry.tiles + 2, 1, 1);
  debugTiles(&geometry, 1);
  


  // debugUpdateTiles(&geometry, 1); // debug at timestep 0
  printf("\n ***** Run sucessful ***** \n");
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

#endif