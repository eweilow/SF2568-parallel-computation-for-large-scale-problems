#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define TIMESTEPS 5

#define TEST_LIST 0
#define DEBUG_LIST 0
#define DEBUG_IDS 0
#define DEBUG_GEOMETRY 0
#define DEBUG_INITIAL_BIRTH 0 // Print debug output in birthRabbit/birthFox in timestep 0
#define DEBUG_SIMULATION 1
#define DEBUG_SIMULATION_DATA 0
#define DEBUG_INDIVIDUAL_ANIMALS 0

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
// https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c

#include "./utils/rand.c"
#include "./utils/list.c"

#include "./data/id.c"
#include "./data/mpi.c"
#include "./data/types.c"
#include "./data/helper.c"
#include "./data/migrations.c"

#include "./simulation/migration.c"
#include "./simulation/birth.c"
#include "./simulation/death.c"
#include "./simulation/init.c"
#include "./simulation/startDay.c"
#include "./simulation/simulateDay.c"

#include "./geometry/rectilinear.c"
#include "./geometry/debug.c"

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

#if DEBUG_IDS
  void debugBinary(u_int64_t value, u_int64_t bits) {
    for(u_int64_t bit = 0; bit < bits; bit++) {
      u_int64_t b = ((u_int64_t) 1) << (bits - bit - 1);
      u_int64_t v = (value & b) == b;
      // printf("%llu %llu %llu\n", bit, value, v);
      printf("%llu", v);
    }
    printf("\n");
  }
#endif

int main(int argc, char **argv)
{
  spawnMPI();

  #if DEBUG_IDS
    u_int64_t id = getNextId((u_int64_t)1);
    debugBinary(id, sizeof(u_int64_t)*8);
    debugBinary(id >> 40, sizeof(u_int64_t)*8);
  #endif

  long rank = 0;

  srand(time(0)); // Deterministic random numbers on all processes

  // initialize geometry data on root process
  TileGeometry geometry = generateIsland(5, 5, 0, 0.1, 1, 1, rank);
  
  #if DEBUG_GEOMETRY
    debugGeometryAdjacency(&geometry);
  #endif

  srand(time(0) + rank); 

  for(long n = 0; n < geometry.tileCount; n++) {
    initializeTile(geometry.tiles + n, TIMESTEPS);
  }
  
  debugTiles(&geometry, 0);

  for(long ts = 1; ts < TIMESTEPS; ts++) {
    for(long n = 0; n < geometry.tileCount; n++) {
      startDataOfNewDay(geometry.tiles + n, ts);
    }
    simulateDay(&geometry, ts);
    applyMigrations(&geometry, ts);
  }
  debugTiles(&geometry, TIMESTEPS - 1);

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

  return 0;
}

#endif