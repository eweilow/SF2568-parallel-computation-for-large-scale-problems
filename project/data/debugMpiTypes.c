void debugMpiTypes(long rank) {
  if(rank == 0) {
    Rabbit rabbit[4];
    rabbit[2].birthDay = (long) 1337;
    rabbit[2].id = (u_int64_t) 420;

    RabbitMigration rabbitMigration[4];
    rabbitMigration[2].rabbit = rabbit[2];
    rabbitMigration[2].toTile = (long) 9001;

    MPI_Send(
      &rabbit,
      4,
      RABBIT_MPI_TYPE,
      1,
      0, // TAG
      MPI_COMM_WORLD
    );

    MPI_Send(
      &rabbitMigration,
      4,
      RABBIT_MIGRATION_MPI_TYPE,
      1,
      0, // TAG
      MPI_COMM_WORLD
    );

    printf("Process 0 sent:\n");
    printf(" rabbit.birthDay = %ld\n", rabbit[2].birthDay);
    printf(" rabbit.id = %llu\n", rabbit[2].id);
    printf(" rabbitMigration.rabbit.birthDay = %ld\n", rabbitMigration[2].rabbit.birthDay);
    printf(" rabbitMigration.rabbit.id = %llu\n", rabbitMigration[2].rabbit.id);
    printf(" rabbitMigration.toTile = %ld\n", rabbitMigration[2].toTile);
    
    Fox fox[4];
    fox[2].birthDay = (long) 1337;
    fox[2].id = (u_int64_t) 420;
    fox[2].hunger = 1.1;

    FoxMigration foxMigration[4];
    foxMigration[2].fox = fox[2];
    foxMigration[2].toTile = (long) 9001;

    MPI_Send(
      &fox,
      4,
      FOX_MPI_TYPE,
      1,
      0, // TAG
      MPI_COMM_WORLD
    );

    MPI_Send(
      &foxMigration,
      4,
      FOX_MIGRATION_MPI_TYPE,
      1,
      0, // TAG
      MPI_COMM_WORLD
    );

    printf("Process 0 sent:\n");
    printf(" fox.birthDay = %ld\n", fox[2].birthDay);
    printf(" fox.id = %llu\n", fox[2].id);
    printf(" fox.hunger = %lf\n", fox[2].hunger);
    printf(" foxMigration.fox.birthDay = %ld\n", foxMigration[2].fox.birthDay);
    printf(" foxMigration.fox.id = %llu\n", foxMigration[2].fox.id);
    printf(" foxMigration.fox.hunger = %lf\n", foxMigration[2].fox.hunger);
    printf(" foxMigration.toTile = %ld\n", foxMigration[2].toTile);
  } else if (rank == 1) {
    Rabbit rabbit[4];
    RabbitMigration rabbitMigration[4];
    MPI_Recv(
        &rabbit,
        4,
        RABBIT_MPI_TYPE,
        0,
        0, // TAG
        MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);
    MPI_Recv(
        &rabbitMigration,
        4,
        RABBIT_MIGRATION_MPI_TYPE,
        0,
        0, // TAG
        MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);

    printf("Process 1 got:\n");
    printf(" rabbit.birthDay = %ld\n", rabbit[2].birthDay);
    printf(" rabbit.id = %llu\n", rabbit[2].id);
    printf(" rabbitMigration.rabbit.birthDay = %ld\n", rabbitMigration[2].rabbit.birthDay);
    printf(" rabbitMigration.rabbit.id = %llu\n", rabbitMigration[2].rabbit.id);
    printf(" rabbitMigration.toTile = %ld\n", rabbitMigration[2].toTile);

    Fox fox[4];
    FoxMigration foxMigration[4];
    MPI_Recv(
        &fox,
        4,
        FOX_MPI_TYPE,
        0,
        0, // TAG
        MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);
    MPI_Recv(
        &foxMigration,
        4,
        FOX_MIGRATION_MPI_TYPE,
        0,
        0, // TAG
        MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);

    printf("Process 1 got:\n");
    printf(" fox.birthDay = %ld\n", fox[2].birthDay);
    printf(" fox.id = %llu\n", fox[2].id);
    printf(" fox.hunger = %lf\n", fox[2].hunger);
    printf(" foxMigration.fox.birthDay = %ld\n", foxMigration[2].fox.birthDay);
    printf(" foxMigration.fox.id = %llu\n", foxMigration[2].fox.id);
    printf(" foxMigration.fox.hunger = %lf\n", foxMigration[2].fox.hunger);
    printf(" foxMigration.toTile = %ld\n", foxMigration[2].toTile);
  }
}