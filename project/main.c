#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define GEOM_W 8
#define GEOM_H 8

#define GEOM_EDGE_WATER_TILES 0
#define GEOM_WATER_PROBABILITY 0.1
#define GEOM_OVERALL_WATER_PROBABILITY 0.0 // Set this to 0.0 to get rid of water everywhere
#define GEOM_CASE 1 // 0, 1, or 2. It's likely good to set GEOM_OVERALL_WATER_PROBABILITY to 0 for the corner cases.

#define TIMESTEPS (365*10)

#define TEST_LIST 0
#define DEBUG_LIST 0
#define DEBUG_IDS 0
#define DEBUG_MPI_TYPES 0
#define DEBUG_GEOMETRY 0
#define DEBUG_INITIAL_BIRTH 0 // Print debug output in birthRabbit/birthFox in timestep 0
#define DEBUG_SIMULATION 0
#define DEBUG_PROCESS_ADJACENCY 0
#define DEBUG_SIMULATION_DATA 0
#define DEBUG_IPC 0
#define DEBUG_LIST_ALLOC 0
#define DEBUG_INDIVIDUAL_ANIMALS 0
#define DEBUG_STEPS 0
#define DEBUG_TILESIMULATION 0
#define DEBUG_CLEAR_MEMORY 0

#define USE_REDUCED_OUTPUT 1

#ifndef SAVE_DATA
  #define SAVE_DATA 0
#endif

// https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#include "./utils/rand.c"
#include "./utils/list.c"

#include "./data/id.c"
#include "./data/types.c"
#include "./data/mpi.c"
#include "./data/debugMpiTypes.c"
#include "./data/helper.c"
#include "./data/migrations.c"

#define FOXES_MEALS_PER_WEEK_MAX ((double)4.0)
#define FOXES_MEALS_PER_WEEK_MIN ((double)2.0)
#define FOXES_EXTRA_MEALS_PER_WEEK (FOXES_MEALS_PER_WEEK_MAX - FOXES_MEALS_PER_WEEK_MIN)
#define FOXES_DECREASE_PER_DAY ((double)(FOXES_MEALS_PER_WEEK_MIN / 7.0))

#include "./simulation/migration.c"
#include "./simulation/birth.c"
#include "./simulation/death.c"
#include "./simulation/init.c"
#include "./simulation/startDay.c"
#include "./simulation/rules.c"
#include "./simulation/tileSimulation.c"
#include "./simulation/simulateDay.c"
#include "./simulation/debugTileSimulation.c"

#include "./geometry/rectilinear.c"
#include "./geometry/debug.c"
#include "./geometry/adjacency.c"

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
  char *endPtr;
  long processesWide = strtol(argv[1], &endPtr, 10);
  long processesHigh = strtol(argv[2], &endPtr, 10);

  long rank, N;
  bool shouldCompute = spawnMPI(argc, argv, &rank, &N, processesWide, processesHigh);

  #if DEBUG_MPI_TYPES
    debugMpiTypes(rank);
    return 0;
  #endif

  if(!shouldCompute) {
    murderMPI();
    return 0;
  }

  List *sendRabbitsLists = (List*) malloc(N * sizeof(List));
  List *sendFoxesList = (List*) malloc(N * sizeof(List));

  #if DEBUG_IDS
    u_int64_t id = getNextId((u_int64_t)1);
    debugBinary(id, sizeof(u_int64_t)*8);
    debugBinary(id >> 40, sizeof(u_int64_t)*8);
  #endif

  clock_t initStart, initEnd, start, end; 
  MPI_Barrier(MPI_COMM_WORLD);
  initStart = clock(); 

  ProcessAdjacency processAdjacency = getAdjacency(rank, processesWide, processesHigh);
  for(long n = 0; n < ADJACENT_PROCESSES; n++) {
    long index = processAdjacency.list[n];
    if(index >= 0) {
      sendRabbitsLists[index] = initList(sizeof(RabbitMigration), 1000);
      sendFoxesList[index] = initList(sizeof(FoxMigration), 1000);
    }
  }

  srand(0); // Deterministic random numbers on all processes

  // initialize geometry data on root process
  TileGeometry geometry = generateIsland(
    GEOM_W, 
    GEOM_H, 
    GEOM_EDGE_WATER_TILES, 
    GEOM_WATER_PROBABILITY, 
    GEOM_OVERALL_WATER_PROBABILITY, 
    processesWide, 
    processesHigh, 
    rank
  );
  
  #if DEBUG_GEOMETRY
    debugGeometryAdjacency(&geometry);
  #endif

  // Init random with time and rank of process
  srand(time(0)); 
  
  #if DEBUG_TILESIMULATION
    debugTileSimulation(&geometry);
    printf("\n ***** DEBUG_TILESIMULATION sucessful ***** \n");
    return 0;
  #endif
  
  
  for(long n = 0; n < geometry.ownTileCount; n++) {
    long i = geometry.ownTileIndices[n];
    initializeTile(geometry.tiles + i, TIMESTEPS);
  }
  
  MPI_Barrier(MPI_COMM_WORLD);
  initEnd = clock(); 
  start = clock();
  // debugTiles(&geometry, 0);

  for(long ts = 1; ts < TIMESTEPS; ts++) {
    for(long n = 0; n < geometry.ownTileCount; n++) {
      long i = geometry.ownTileIndices[n];
      startDataOfNewDay(geometry.tiles + i, ts);
    }

    #if DEBUG_STEPS
    if(rank == 0) {
      printf("Simulating day %ld\n", ts);
    }
    #endif
    simulateDay(&geometry, ts);
    #if DEBUG_STEPS
    if(rank == 0) {
      printf("Applying migrations for day %ld\n", ts);
    }
    #endif
    applyMigrations(processAdjacency, sendRabbitsLists, sendFoxesList, &geometry, ts);

  }

  MPI_Barrier(MPI_COMM_WORLD);
  end = clock();

  if(rank == 0) {
    double init_time_taken = (double)(initEnd - initStart) / (double)(CLOCKS_PER_SEC);
    double time_taken = (double)(end - start) / (double)(CLOCKS_PER_SEC);
    printf("%ld x %ld processes: tstart = %.8lf, t = %.8lf (%ld)\n", processesWide, processesHigh, init_time_taken, time_taken, (long)CLOCKS_PER_SEC);
  }

  #if USE_REDUCED_OUTPUT
  for(long n = 0; n < geometry.ownTileCount; n++) {
    long i = geometry.ownTileIndices[n];
    reduceOutput(geometry.tiles + i, TIMESTEPS-1);
  }
  #endif

  murderMPI();

  #if SAVE_DATA
    //printf("\n ***** Saving tiles ***** \n");
    char fileName[50];
    #if USE_REDUCED_OUTPUT
      sprintf(fileName, "./data_p%ld.red.bin", rank);
    #else
      sprintf(fileName, "./data_p%ld.bin", rank);
    #endif
    FILE *fp = fopen(fileName, "w");

    long reduced = 0;
    #if USE_REDUCED_OUTPUT
      reduced = 1;
    #endif
    fwrite(&reduced, sizeof(long), 1, fp);

    fwrite(&rank, sizeof(long), 1, fp);
    fwrite(&geometry.tileCount, sizeof(long), 1, fp);
    fwrite(&geometry.tileSize, sizeof(double), 1, fp);
#if USE_REDUCED_OUTPUT
    long steps = (long)TIMESTEPS;
    fwrite(&steps, sizeof(long), 1, fp);
#endif

    long rabbitSize = sizeof(Rabbit);
    fwrite(&rabbitSize, sizeof(long), 1, fp);
    long foxSize = sizeof(Fox);
    fwrite(&foxSize, sizeof(long), 1, fp);

    for(long n = 0; n < geometry.tileCount; n++) {
      Tile tile = geometry.tiles[n];
      fwrite(&tile.x, sizeof(double), 1, fp);
      fwrite(&tile.y, sizeof(double), 1, fp);
      fwrite(&tile.id, sizeof(u_int64_t), 1, fp);
      fwrite(&tile.process, sizeof(long), 1, fp);
      fwrite(&tile.isOwnedByThisProcess, sizeof(bool), 1, fp);
      fwrite(&tile.isWaterTile, sizeof(bool), 1, fp);
      
      if(tile.isOwnedByThisProcess) {
        fwrite(&tile.historicalDataCount, sizeof(long), 1, fp);
        for(long ts = 0; ts < tile.historicalDataCount; ts++) {
          TileData data = tile.historicalData[ts];
          fwrite(&data.vegetation, sizeof(double), 1, fp);
#if USE_REDUCED_OUTPUT
          fwrite(&data.foxCount, sizeof(long), 1, fp);
          fwrite(&data.rabbitCount, sizeof(long), 1, fp);
          fwrite(&data.totalFoxHunger, sizeof(double), 1, fp);
          fwrite(&data.totalFoxAge, sizeof(long), 1, fp);
          fwrite(&data.totalRabbitAge, sizeof(long), 1, fp);
          fwrite(&data.maxFoxAge, sizeof(long), 1, fp);
          fwrite(&data.maxRabbitAge, sizeof(long), 1, fp);
#else

          long rabbitsCount;
          Rabbit *rabbits;
          list_read(&data.rabbitsList, &rabbitsCount, (void**)&rabbits);
          fwrite(&rabbitsCount, sizeof(long), 1, fp);
          fwrite(rabbits, sizeof(Rabbit), rabbitsCount, fp);

          long foxesCount;
          Fox *foxes;
          list_read(&data.foxesList, &foxesCount, (void**)&foxes);
          fwrite(&foxesCount, sizeof(long), 1, fp);
          fwrite(foxes, sizeof(Fox), foxesCount, fp);
#endif
        }
      }
    }
    fclose(fp);
    //printf("\n ***** Data saved ***** \n");
  #endif

  #if DEBUG_CLEAR_MEMORY
  for(long n = 0; n < geometry.tileCount; n++) {
    Tile tile = geometry.tiles[n];
    if(tile.isOwnedByThisProcess) {
      for(long ts = 0; ts < tile.historicalDataCount; ts++) {
        list_clear(&tile.historicalData[ts].rabbitsList);
        list_clear(&tile.historicalData[ts].foxesList);
      }

      free(tile.historicalData);

      list_clear(&tile.rabbitMigrationsList);
      list_clear(&tile.foxMigrationsList);
      
      if(tile.adjacency.immediatelyAdjacentTileIndices != NULL) {
        free(tile.adjacency.immediatelyAdjacentTileIndices);
      }
      if(tile.adjacency.indirectlyAdjacentTileIndices != NULL) {
        free(tile.adjacency.indirectlyAdjacentTileIndices);
      }
    }
  }
  for(long n = 0; n < ADJACENT_PROCESSES; n++) {
    long index = processAdjacency.list[n];
    if(index >= 0) {
      list_clear(&sendRabbitsLists[index]);
      list_clear(&sendFoxesList[index]);
    }
  }
  if(geometry.tiles != NULL) {
    free(geometry.tiles);
  }
  if(geometry.ownTileIndices != NULL) {
    free(geometry.ownTileIndices);
  }
  #endif

  return 0;
}

#endif
