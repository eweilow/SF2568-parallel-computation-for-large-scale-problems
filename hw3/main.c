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
#define DEBUG_STEPS 0

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

void debug_output(INT p, double* x, int I) {
  printf("-- p %d --\n", p);
  for (int i = 0; i < I; i++) {
    printf("%d: %f\n", i, x[i]);
  }
}

void print_debug_output(INT p, double* x, int P, int I, int s) {
  if(DEBUG_STEPS) {
    MPI_Barrier(MPI_COMM_WORLD);

    if(p == 0) {
      printf("\n\nDebug output in step %d\n", s);
      debug_output(p, x, I + 1);
    }

    for(int i = 1; i < P; i++) {
      MPI_Barrier(MPI_COMM_WORLD);
      if(p == 0) {
        int recvI;
        MPI_Recv(
          &recvI,
          1,
          MPI_INT,
          i,
          0, // TAG
          MPI_COMM_WORLD,
          MPI_STATUS_IGNORE);
        
        double* recvX = (double*)malloc(sizeof(double) * (recvI + 1));
        MPI_Recv(
          recvX,
          recvI + 1,
          MPI_DOUBLE,
          i,
          0, // TAG
          MPI_COMM_WORLD,
          MPI_STATUS_IGNORE);

        debug_output(i, recvX, recvI + 1);

        free(recvX);
      } else if(p == i) {
        MPI_Send(
          &I,
          1,
          MPI_INT,
          0,
          0, // TAG
          MPI_COMM_WORLD);
        MPI_Send(
          x,
          I+1,
          MPI_DOUBLE,
          0,
          0, // TAG
          MPI_COMM_WORLD);
      }
    }
    /*
      usleep(100);
      if (i == p) {
        if(p == 0) {
          printf("\n\nDebug output in step %d\n", s);
        }
        printf("-- p %d --\n", p);
        for (int i = 0; i < I + 2; i++) {
          printf("%d %d: %f\n", p, i, x[i]);
        }
        fflush(NULL);
      }
    }
    */
    MPI_Barrier(MPI_COMM_WORLD);
  }
}

void main(int argc, char **argv)
{
  int P, p;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &P);
  MPI_Comm_rank(MPI_COMM_WORLD, &p);

  if(p == 0) {
    for(int i = 0; i < argc; i++) {
      printf("argv[%d] = '%s'\n", i, argv[i]);
    }
  }

  char *endPtr;
  int N = strtol(argv[1], &endPtr, 10);
  // local size. Modify if P does not divide N
  int R = N % P;
  int I = N/P - (N/P % 2); // + (p < R ? 1 : 0);
  int r = N - P*I;

  int nIncr = r/2;
  
  if(p == 0) {
    if(p % 2 == 0 && p/2 < nIncr) {
      I += 2;
    }

    printf("N: %d\n", N);
    printf("I: %d\n", I);
    printf("r: %d\n", r);
  }

  if(p == P-1) {
    I += r - nIncr * 2;
  }

  for(int i = 0; i < P; i++) {
    MPI_Barrier(MPI_COMM_WORLD);
    if(p == i) {
      printf("p %d: %d\n", p, I);
    }
  }
    
  // random number generator initialization
  srand(p+1);
  // data generation
  double* x = (double*) malloc(sizeof(double) * (I + 1));
  for (int i = 0; i < I; i++) {
    x[i] = ((double) rand())/((double)RAND_MAX + 1.0);
  }
  x[I] = -1.0;

  print_debug_output(p, x, P, I, -1);

  for(int s = 0; s < N; s++) {
    int evenPhase = s % 2 == 0;
    int oddPhase = s % 2 == 1;
    
    if(oddPhase) {
      if(DEBUG_STEPS && p == 0) {
        printf("transmitting leftward\n");
      }

      if(p % 2 == 0) { // Receive from right
        if(p + 1 < P) {
          x[I] = custom_receive(p, p + 1);
        }
      } else { // Send to left
        if(p > 0) {
          custom_send(x[0], p, p - 1);
        }
      }
      if(p % 2 == 1) { // Receive from right
        if(p + 1 < P) {
          x[I] = custom_receive(p, p + 1);
        }
      } else { // Send to left
        if(p > 0) {
          custom_send(x[0], p, p - 1);
        }
      }

      print_debug_output(p, x, P, I, s);
    }

/*
    
    */
    for(int i = evenPhase ? 0 : 1; i < I - (p == P-1 ? 1 : 0); i += 2) {
      if(x[i] > x[i+1]) {
        double tmp = x[i+1];
        x[i+1] = x[i];
        x[i] = tmp;
      }
    }
    print_debug_output(p, x, P, I, s);
    
    if(oddPhase) {
      if(DEBUG_STEPS && p == 0) {
        printf("transmitting rightward\n");
      }

      if(p % 2 == 0) { // Send to right
        if(p + 1 < P) {
          custom_send(x[I], p, p + 1);
        }
      } else { // Receive from left
        if(p > 0) {
          x[0] = custom_receive(p, p - 1);
        }
      }
      if(p % 2 == 1) { // Send to right
        if(p + 1 < P) {
          custom_send(x[I], p, p + 1);
        }
      } else { // Receive from left
        if(p > 0) {
          x[0] = custom_receive(p, p - 1);
        }
      }

      print_debug_output(p, x, P, I, s);
    }
  }

  
  print_debug_output(p, x, P, I, N+1);

  MPI_Finalize();

  /*

  // TODO: Initial Local sort ????????????????????????????????????????????
  // HERE
  int evenprocess = (myp % 2);
	int evenphase = 1;
	for (int step = 0; step<N; step++) {
    double rec;
    double smallest = x[0];
    double largest = x[I-1];
		 if (~evenprocess) {

		   if (~evenphase) {
         custom_send(largest, myp, myp+1);
		     rec = custom_receive(myp, myp+1);
		   } else {
		     custom_send(smallest, myp, myp-1);
		     rec = custom_receive(myp, myp-1);
		   }
		   if (x[i] < rec)
        rec = x[i];
       } else {

		   if (evenphase) {
         rec = custom_receive(myp, myp-1);
		     custom_send(smallest, myp, myp-1);
		   } else {
		     rec = custom_receive(myp, myp+1);
		     custom_send(largest, myp, myp+1);
		   }
		   if (x[i] > rec)
		      rec = x[i];
     }
     evenphase = ~evenphase;
 }*/

  // printf("sorted: %s\n", x);
}
