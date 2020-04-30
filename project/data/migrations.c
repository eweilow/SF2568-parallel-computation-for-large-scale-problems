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
    char rabbitId[50];
    formatGlobalId(migrations[n].rabbit.id, rabbitId);

    long toTileIndex = migrations[n].toTile;
    Tile *toTile = geometry->tiles + (toTileIndex);

    List *toList = getRabbits(toTile, ts);

    bool isLocalMigration = tile->process == toTile->process;
#if DEBUG_IPC
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
    char foxId[50];
    formatGlobalId(migrations[n].fox.id, foxId);

    long toTileIndex = migrations[n].toTile;
    Tile *toTile = geometry->tiles + (toTileIndex);

    List *toList = getFoxes(toTile, ts);

    bool isLocalMigration = tile->process == toTile->process;
#if DEBUG_IPC
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

  printf("[SEND] SENDING %ld RABBITS FROM %ld TO %ld\n", len, fromProcess, toProcess);

  if(len == 0) {
    return;
  }

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

  printf("[SEND] SENDING %ld FOXES FROM %ld TO %ld\n", len, fromProcess, toProcess);

  if(len == 0) {
    return;
  }


  list_clear(list);
}

void receiveRabbits(long at, long from) {
  if(from == -1) {
    return;
  }

  printf("[RECV] RECEIVING RABBITS AT %ld FROM %ld\n", at, from);
}

void receiveFoxes(long at, long from) {
  if(from == -1) {
    return;
  }

  printf("[RECV] RECEIVING FOXES AT %ld FROM %ld\n", at, from);
}


void communicate(
  ProcessAdjacency processAdjacency,
  List* sendLists,
  void (*send)(List* sendLists, long from, long to) ,
  void (*recv)(long at, long from) 
) {
  long col = processAdjacency.col;
  long row = processAdjacency.row;

  printf("NW/SE\n");
  if(row % 2 == 0) {
    send(sendLists, processAdjacency.current, processAdjacency.downRight);
    recv(processAdjacency.current, processAdjacency.downRight);
  } else {
    recv(processAdjacency.current, processAdjacency.upLeft);
    send(sendLists, processAdjacency.current, processAdjacency.upLeft);
  }

  printf("N/S\n");
  if(row % 2 == 0) {
    send(sendLists, processAdjacency.current, processAdjacency.down);
    recv(processAdjacency.current, processAdjacency.down);
  } else {
    recv(processAdjacency.current, processAdjacency.up);
    send(sendLists, processAdjacency.current, processAdjacency.up);
  }

  printf("NE/SW\n");
  if(row % 2 == 0) {
    send(sendLists, processAdjacency.current, processAdjacency.downLeft);
    recv(processAdjacency.current, processAdjacency.downLeft);
  } else {
    recv(processAdjacency.current, processAdjacency.upRight);
    send(sendLists, processAdjacency.current, processAdjacency.upRight);
  }

  printf("W/E\n");
  if(col % 2 == 0) {
    send(sendLists, processAdjacency.current, processAdjacency.right);
    recv(processAdjacency.current, processAdjacency.right);
  } else {
    recv(processAdjacency.current, processAdjacency.left);
    send(sendLists, processAdjacency.current, processAdjacency.left);
  }
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

  communicate(processAdjacency, sendRabbitsLists, &sendRabbits, &receiveRabbits);
  communicate(processAdjacency, sendFoxesList, &sendFoxes, &receiveFoxes);
}