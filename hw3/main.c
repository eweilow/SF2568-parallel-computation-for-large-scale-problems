#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "mpi.h"
#include <unistd.h>

#define INT int64_t

#define DEBUG_COMM 0

double custom_receive(INT current, INT p)
{
  if (DEBUG_COMM)
  {
    printf("WAIT %d <- %d\n", current, p);
  }
  double u0;
  MPI_Recv(
      &u0,
      1,
      MPI_DOUBLE,
      p,
      0, // TAG
      MPI_COMM_WORLD,
      MPI_STATUS_IGNORE);
  if (DEBUG_COMM)
  {
    printf("DONE WAIT %d <- %d, %.8lf\n", current, p, u0);
  }
  return u0;
}

void custom_send(double data, INT current, INT p)
{
  if (DEBUG_COMM)
  {
    printf("SEND %d -> %d, %.8lf\n", current, p, data);
  }
  MPI_Send(
      &data,
      1,
      MPI_DOUBLE,
      p,
      0, // TAG
      MPI_COMM_WORLD);
  if (DEBUG_COMM)
  {
    printf("DONE SEND %d -> %d\n", current, p);
  }
}

int main(int argc, char **argv)
{
  int P;
  int myrank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &P);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  /* Find problem size N from command line */
  if (argc < 2)
    printf("Too few args in. argc - %s\n", argc);

  int N = atoi(argv[1]);
  printf("Startup info:\n P - %s \n myrank - %s \n N - %s", P, myrank, N);
  /* local size. Modify if P does not divide N */
  int I = N/P;
  /* random number generator initialization */
  srandom(myrank+1);
  /* data generation */
  double* x = (double*) malloc(sizeof(double)*I);
  for (int i = 0; i < I; i++)
    x[i] = (double) i; //((double) random(0,1))/(RAND_MAX+1);

  int evenprocess = (myrank % 2);
	int evenphase = 1;
	for (int step = 0; step<N; step++) {
    double rec;
    int i = 0;
		 if (~evenprocess) {

		   if (~evenphase) {
         custom_send(x[i], myrank, myrank+1);
		     rec = custom_receive(myrank, myrank+1);
		   } else {
		     custom_send(x[i], myrank, myrank-1);
		     rec = custom_receive(myrank, myrank-1);
		   }
		   if (x[i] < rec)
        rec = x[i];

       } else {

		   if (evenphase) {
         rec = custom_receive(myrank, myrank-1);
		     custom_send(x[i], myrank, myrank-1);
		   } else {
		     rec = custom_receive(myrank, myrank+1);
		     custom_send(x[i], myrank, myrank+1);
		   }
		   if (x[i] > rec)
		      rec = x[i];
     }
     evenphase = ~evenphase;
 }

printf("sorted: %s\n", x);
}
