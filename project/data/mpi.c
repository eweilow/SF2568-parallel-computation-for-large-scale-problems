MPI_Datatype RABBIT_MPI_TYPE;
void defineRabbitType() {
  int blockLengths[2];
  MPI_Aint displacementsInStruct[2];
  MPI_Datatype types[2];
  
  Rabbit placeholder;

  blockLengths[0] = 1;
  displacementsInStruct[0] = (MPI_Aint)((long)&placeholder.birthDay - (long)&placeholder);
  types[0] = MPI_LONG;

  blockLengths[1] = 1;
  displacementsInStruct[1] = (MPI_Aint)((long)&placeholder.id - (long)&placeholder);
  types[1] = MPI_UINT64_T;

  int output = MPI_Type_create_struct(
    2, 
    blockLengths, 
    displacementsInStruct, 
    types, 
    &RABBIT_MPI_TYPE
  );
  MPI_Type_commit(&RABBIT_MPI_TYPE);
}

MPI_Datatype RABBIT_MIGRATION_MPI_TYPE;
void defineRabbitMigrationType() {
  int blockLengths[2];
  MPI_Aint displacementsInStruct[2];
  MPI_Datatype types[2];
  
  RabbitMigration placeholder;

  blockLengths[0] = 1;
  displacementsInStruct[0] = (MPI_Aint)((long)&placeholder.rabbit - (long)&placeholder);
  types[0] = RABBIT_MPI_TYPE;

  blockLengths[1] = 1;
  displacementsInStruct[1] = (MPI_Aint)((long)&placeholder.toTile - (long)&placeholder);
  types[1] = MPI_LONG;

  int output = MPI_Type_create_struct(
    2, 
    blockLengths, 
    displacementsInStruct, 
    types, 
    &RABBIT_MIGRATION_MPI_TYPE
  );
  MPI_Type_commit(&RABBIT_MIGRATION_MPI_TYPE);
}

MPI_Datatype FOX_MPI_TYPE;
void defineFoxType() {
  int blockLengths[3];
  MPI_Aint displacementsInStruct[3];
  MPI_Datatype types[3];
  
  Fox placeholder;

  blockLengths[0] = 1;
  displacementsInStruct[0] = (MPI_Aint)((long)&placeholder.birthDay - (long)&placeholder);
  types[0] = MPI_LONG;

  blockLengths[1] = 1;
  displacementsInStruct[1] = (MPI_Aint)((long)&placeholder.id - (long)&placeholder);
  types[1] = MPI_UINT64_T;

  blockLengths[2] = 1;
  displacementsInStruct[2] = (MPI_Aint)((long)&placeholder.hunger - (long)&placeholder);
  types[2] = MPI_DOUBLE;

  int output = MPI_Type_create_struct(
    3, 
    blockLengths, 
    displacementsInStruct, 
    types, 
    &FOX_MPI_TYPE
  );
  MPI_Type_commit(&FOX_MPI_TYPE);
}

MPI_Datatype FOX_MIGRATION_MPI_TYPE;
void defineFoxMigrationType() {
  int blockLengths[2];
  MPI_Aint displacementsInStruct[2];
  MPI_Datatype types[2];
  
  FoxMigration placeholder;

  blockLengths[0] = 1;
  displacementsInStruct[0] = (MPI_Aint)((long)&placeholder.fox - (long)&placeholder);
  types[0] = FOX_MPI_TYPE;

  blockLengths[1] = 1;
  displacementsInStruct[1] = (MPI_Aint)((long)&placeholder.toTile - (long)&placeholder);
  types[1] = MPI_LONG;

  int output = MPI_Type_create_struct(
    2, 
    blockLengths, 
    displacementsInStruct, 
    types, 
    &FOX_MIGRATION_MPI_TYPE
  );
  MPI_Type_commit(&FOX_MIGRATION_MPI_TYPE);
}

bool spawnMPI(int argc, char **argv, long *rank, long *processCount, long processesWide, long processesHigh) {
  MPI_Init(&argc, &argv);

  int P, p;
  MPI_Comm_size(MPI_COMM_WORLD, &P);
  MPI_Comm_rank(MPI_COMM_WORLD, &p);

  *processCount = (long)P;
  *rank = (long)p;

  defineRabbitType();
  defineRabbitMigrationType();
  defineFoxType();
  defineFoxMigrationType();

  long neededProcesses = processesHigh * processesWide;
  if(*processCount < neededProcesses) {
    if(rank == 0) {
      printf("Exiting: not enough processes. Need %ld x %ld = %ld, but have %ld.", processesWide, processesHigh, neededProcesses, processCount);
    }
    return false;
    // exit(1);
  }

  if(*rank >= neededProcesses) {
    printf("Skipping calculating on process %ld.\n", *rank);
    return false;
  }

  return true;
}

void murderMPI() {
  MPI_Finalize();
}