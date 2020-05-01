void applyRabbitMigrations(
  List* sendRabbitsLists,
  TileGeometry *geometry,
  Tile *tile,
  long ts
) {
  List *migrationsList = &tile->rabbitMigrationsList;
  long len;
  RabbitMigration *migrations;
  list_read(migrationsList, &len, (void**)&migrations);
  for(long n = 0; n < len; n++) {

    long toTileIndex = migrations[n].toTile;
    Tile *toTile = geometry->tiles + (toTileIndex);

    List *toList = getRabbits(toTile, ts);

    bool isLocalMigration = tile->process == toTile->process;
#if DEBUG_IPC
    char rabbitId[50];
    formatGlobalId(migrations[n].rabbit.id, rabbitId);
    if(!isLocalMigration) {
      printf("[DATA.%ld] rabbit.%-12s moves from %+4llu to %+4llu - a migration from process %ld to %ld.\n", ts, rabbitId, tile->id, toTile->id, tile->process, toTile->process);
    } else {
      printf("[DATA.%ld] rabbit.%-12s moves from %+4llu to %+4llu - a migration on process %ld.\n", ts, rabbitId, tile->id, toTile->id, tile->process);
    }
#endif
    if(isLocalMigration) {
      list_insert(toList, &migrations[n].rabbit);
    } else {
      list_insert(sendRabbitsLists + toTile->process, &migrations[n].rabbit);
    }
  }
  list_clear(migrationsList);
}

void applyFoxMigrations(
  List* sendFoxesList,
  TileGeometry *geometry,
  Tile *tile,
  long ts
) {
  List *migrationsList = &tile->foxMigrationsList;
  long len;
  FoxMigration *migrations;
  list_read(migrationsList, &len, (void**)&migrations);
  for(long n = 0; n < len; n++) {

    long toTileIndex = migrations[n].toTile;
    Tile *toTile = geometry->tiles + (toTileIndex);

    List *toList = getFoxes(toTile, ts);

    bool isLocalMigration = tile->process == toTile->process;
#if DEBUG_IPC
    char foxId[50];
    formatGlobalId(migrations[n].fox.id, foxId);
    if(!isLocalMigration) {
      printf("[DATA.%ld]    fox.%-12s moves from %+4llu to %+4llu - a migration from process %ld to %ld.\n", ts, foxId, tile->id, toTile->id, tile->process, toTile->process);
    } else {
      printf("[DATA.%ld]    fox.%-12s moves from %+4llu to %+4llu - a migration on process %ld.\n", ts, foxId, tile->id, toTile->id, tile->process);
    }
#endif
    if(isLocalMigration) {
      list_insert(toList, &migrations[n].fox);
    } else {
      list_insert(sendFoxesList + toTile->process, &migrations[n].fox);
    }
  }
  list_clear(migrationsList);
}


void applyMigrationsOfTile(
  List* sendRabbitsLists,
  List* sendFoxesList,
  TileGeometry *geometry,
  Tile *tile,
  long ts
) {
  applyRabbitMigrations(sendRabbitsLists, geometry, tile, ts);
  applyFoxMigrations(sendFoxesList, geometry, tile, ts);
}


void sendRabbits(
  List* sendRabbitsLists,
  long fromProcess,
  long toProcess
) {
  if(toProcess < 0) {
    return;
  }

  List *list = sendRabbitsLists + toProcess;

  long len;
  RabbitMigration *migrations;
  list_read(list, &len, (void**)&migrations);


#if DEBUG_IPC
  printf("[SEND.%ld] SENDING %ld RABBITS FROM %ld TO %ld\n", fromProcess, len, fromProcess, toProcess);
#endif

  MPI_Send(&len, 1, MPI_LONG, toProcess, 0, MPI_COMM_WORLD);
  MPI_Send(migrations, len, RABBIT_MIGRATION_MPI_TYPE, toProcess, 0, MPI_COMM_WORLD);

  list_clear(list);
}

void sendFoxes(
  List* sendFoxesLists,
  long fromProcess,
  long toProcess
) {
  if(toProcess < 0) {
    return;
  }

  List *list = sendFoxesLists + toProcess;

  long len;
  FoxMigration *migrations;
  list_read(list, &len, (void**)&migrations);


#if DEBUG_IPC
  printf("[SEND.%ld] SENDING %ld FOXES FROM %ld TO %ld\n", fromProcess, len, fromProcess, toProcess);
#endif

  MPI_Send(&len, 1, MPI_LONG, toProcess, 0, MPI_COMM_WORLD);
  MPI_Send(migrations, len, FOX_MIGRATION_MPI_TYPE, toProcess, 0, MPI_COMM_WORLD);

  list_clear(list);
}

void receiveRabbits(long* outputDataLen, void **outputDataPtr, long at, long from) {
  if(from == -1) {
    return;
  }


#if DEBUG_IPC
  printf("[RECV.%ld] RECEIVING RABBITS AT %ld FROM %ld\n", at, at, from);
#endif

  long len;
  MPI_Recv(&len, 1, MPI_LONG, from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  RabbitMigration* data = (RabbitMigration*) malloc(sizeof(RabbitMigration) * len);
  MPI_Recv(data, len, RABBIT_MIGRATION_MPI_TYPE, from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  // MPI commands
  
#if DEBUG_IPC
  printf("[RECV.%ld] RECEIVED %ld RABBITS AT %ld FROM %ld\n", at, len, at, from);
#endif
  *outputDataLen = len;
  *outputDataPtr = data;
}

void receiveFoxes(long* outputDataLen, void **outputDataPtr, long at, long from) {
  if(from == -1) {
    return;
  }


#if DEBUG_IPC
  printf("[RECV.%ld] RECEIVING FOXES AT %ld FROM %ld\n", at, at, from);
#endif

  long len;
  MPI_Recv(&len, 1, MPI_LONG, from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  FoxMigration* data = (FoxMigration*) malloc(sizeof(FoxMigration) * len);
  MPI_Recv(data, len, FOX_MIGRATION_MPI_TYPE, from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


#if DEBUG_IPC
  printf("[RECV.%ld] RECEIVED %ld FOXES AT %ld FROM %ld\n", at, len, at, from);
#endif
  *outputDataLen = len;
  *outputDataPtr = data;
}

void applyReceivedRabbitMigrations(
  TileGeometry *geometry, 
  long *lenPtr, void **dataPtr,
  long ts
) {
  long len = *lenPtr;
  RabbitMigration* migrations = (RabbitMigration*)(*dataPtr);

  for(long n = 0; n < len; n++) {
    RabbitMigration migration = migrations[n];
    long toTile = migration.toTile;
    List *toList = getRabbits(geometry->tiles + toTile, ts);
    list_insert(toList, &migration.rabbit);
  }

  free(migrations);
  *lenPtr = 0;
  *dataPtr = NULL;
}

void applyReceivedFoxMigrations(
  TileGeometry *geometry, 
  long *lenPtr, void **dataPtr,
  long ts
) {
  long len = *lenPtr;
  FoxMigration* migrations = (FoxMigration*)(*dataPtr);

  for(long n = 0; n < len; n++) {
    FoxMigration migration = migrations[n];
    long toTile = migration.toTile;
    List *toList = getFoxes(geometry->tiles + toTile, ts);
    list_insert(toList, &migration.fox);
  }

  free(migrations);
  *lenPtr = 0;
  *dataPtr = NULL;
}

void communicate(
  TileGeometry *geometry, 
  ProcessAdjacency processAdjacency,
  List* sendLists,
  long ts,
  void (*send)(List* sendLists, long from, long to),
  void (*recv)(long* outputDataLen, void **outputDataPtr, long at, long from),
  void (*applyMigrations)(TileGeometry *geometry, long *len, void **data, long ts)
) {
  long col = processAdjacency.col;
  long row = processAdjacency.row;

  long currentDataLen = 0;
  void *currentDataPtr = NULL;

  MPI_Barrier(MPI_COMM_WORLD);
  if(row % 2 == 0) {
    send(sendLists, processAdjacency.current, processAdjacency.downRight);
    recv(&currentDataLen, &currentDataPtr, processAdjacency.current, processAdjacency.downRight);
  } else {
    recv(&currentDataLen, &currentDataPtr, processAdjacency.current, processAdjacency.upLeft);
    send(sendLists, processAdjacency.current, processAdjacency.upLeft);
  }

  applyMigrations(geometry, &currentDataLen, &currentDataPtr, ts);
  MPI_Barrier(MPI_COMM_WORLD);

  if(row % 2 == 1) {
    send(sendLists, processAdjacency.current, processAdjacency.downRight);
    recv(&currentDataLen, &currentDataPtr, processAdjacency.current, processAdjacency.downRight);
  } else {
    recv(&currentDataLen, &currentDataPtr, processAdjacency.current, processAdjacency.upLeft);
    send(sendLists, processAdjacency.current, processAdjacency.upLeft);
  }
  
  applyMigrations(geometry, &currentDataLen, &currentDataPtr, ts);
  MPI_Barrier(MPI_COMM_WORLD);

  if(row % 2 == 0) {
    send(sendLists, processAdjacency.current, processAdjacency.down);
    recv(&currentDataLen, &currentDataPtr, processAdjacency.current, processAdjacency.down);
  } else {
    recv(&currentDataLen, &currentDataPtr, processAdjacency.current, processAdjacency.up);
    send(sendLists, processAdjacency.current, processAdjacency.up);
  }

  applyMigrations(geometry, &currentDataLen, &currentDataPtr, ts);
  MPI_Barrier(MPI_COMM_WORLD);

  if(row % 2 == 1) {
    send(sendLists, processAdjacency.current, processAdjacency.down);
    recv(&currentDataLen, &currentDataPtr, processAdjacency.current, processAdjacency.down);
  } else {
    recv(&currentDataLen, &currentDataPtr, processAdjacency.current, processAdjacency.up);
    send(sendLists, processAdjacency.current, processAdjacency.up);
  }
  
  applyMigrations(geometry, &currentDataLen, &currentDataPtr, ts);
  MPI_Barrier(MPI_COMM_WORLD);

  if(row % 2 == 0) {
    send(sendLists, processAdjacency.current, processAdjacency.downLeft);
    recv(&currentDataLen, &currentDataPtr, processAdjacency.current, processAdjacency.downLeft);
  } else {
    recv(&currentDataLen, &currentDataPtr, processAdjacency.current, processAdjacency.upRight);
    send(sendLists, processAdjacency.current, processAdjacency.upRight);
  }

  applyMigrations(geometry, &currentDataLen, &currentDataPtr, ts);
  MPI_Barrier(MPI_COMM_WORLD);

  if(row % 2 == 1) {
    send(sendLists, processAdjacency.current, processAdjacency.downLeft);
    recv(&currentDataLen, &currentDataPtr, processAdjacency.current, processAdjacency.downLeft);
  } else {
    recv(&currentDataLen, &currentDataPtr, processAdjacency.current, processAdjacency.upRight);
    send(sendLists, processAdjacency.current, processAdjacency.upRight);
  }

  applyMigrations(geometry, &currentDataLen, &currentDataPtr, ts);
  MPI_Barrier(MPI_COMM_WORLD);

  if(col % 2 == 0) {
    send(sendLists, processAdjacency.current, processAdjacency.right);
    recv(&currentDataLen, &currentDataPtr, processAdjacency.current, processAdjacency.right);
  } else {
    recv(&currentDataLen, &currentDataPtr, processAdjacency.current, processAdjacency.left);
    send(sendLists, processAdjacency.current, processAdjacency.left);
  }

  applyMigrations(geometry, &currentDataLen, &currentDataPtr, ts);
  MPI_Barrier(MPI_COMM_WORLD);

  if(col % 2 == 1) {
    send(sendLists, processAdjacency.current, processAdjacency.right);
    recv(&currentDataLen, &currentDataPtr, processAdjacency.current, processAdjacency.right);
  } else {
    recv(&currentDataLen, &currentDataPtr, processAdjacency.current, processAdjacency.left);
    send(sendLists, processAdjacency.current, processAdjacency.left);
  }

  applyMigrations(geometry, &currentDataLen, &currentDataPtr, ts);
  MPI_Barrier(MPI_COMM_WORLD);
}

void applyMigrations(
  ProcessAdjacency processAdjacency,
  List* sendRabbitsLists,
  List* sendFoxesList,
  TileGeometry *geometry,
  long ts
) {
  for(long n = 0; n < geometry->tileCount; n++) {
    applyMigrationsOfTile(sendRabbitsLists, sendFoxesList, geometry, geometry->tiles + n, ts);
  }

  communicate(geometry, processAdjacency, sendRabbitsLists, ts, &sendRabbits, &receiveRabbits, &applyReceivedRabbitMigrations);
  communicate(geometry, processAdjacency, sendFoxesList, ts, &sendFoxes, &receiveFoxes, &applyReceivedFoxMigrations);
}