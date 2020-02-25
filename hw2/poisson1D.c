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

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* define problem to be solved */
#define N 200      /* number of inner grid points */
#define SMX 250000 /* number of iterations */
#define A 0

#define PI M_PI

/* implement coefficient functions */
double r(const double x)
{
  return -x;
}

double f(const double x)
{
  return 30.0 - 10.0 * x * r(x) * (x - 1.0) * (x - 0.5) - 60.0 * x;
}

double u_analytic(const double x)
{
  return -10.0 * x * (x - 1.0) * (x - 0.5);
}

INT globalIndex(INT L, INT R, INT p, INT localIndex)
{
  return L * p + MIN(R, p) + localIndex;
}

double custom_recieve(INT current, INT p)
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

void main(int argc, char **argv)
{
  /* Initialize MPI */
  int P, p, M, L, R, I;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &P);
  MPI_Comm_rank(MPI_COMM_WORLD, &p);

  if (N < P)
  {
    printf("Too few discretization points...\n");
    // exit(1);
  }
  /* Compute local indices for data distribution */
  M = N + (P - 1) * A;
  L = M / P;
  R = M % P;
  I = p < R ? L + 1 : L;

  double h = 1.0 / (double)(N + 1);

  INT n = (INT)N;
  printf("rank: %d, P: %d, M: %d, L: %d, R: %d, I: %d\n", p, P, M, L, R, I);
  MPI_Barrier(MPI_COMM_WORLD);
  printf("rank %d: %d, %d ... %d\n", p, I, globalIndex(L, R, p, 0), globalIndex(L, R, p, I - 1));
  MPI_Barrier(MPI_COMM_WORLD);

  /* arrays */
  double *unew = (double *)calloc(I, sizeof(double));
  double *ff = (double *)calloc(I, sizeof(double));
  double *rr = (double *)calloc(I, sizeof(double));
  double *u = (double *)calloc(I + 2, sizeof(double));

  for (int i = 0; i < I; i++)
  {
    INT cv = 1 + globalIndex(L, R, p, i);
    double xn = cv * h;
    ff[i] = f(xn);
    rr[i] = r(xn);
  }

  double last = 0.0;
  /* Jacobi iteration */
  for (INT step = 0; step < SMX; step++)
  {
    /* local iteration step */
    for (int i = 0; i < I; i++)
    {
      unew[i] = (u[i] + u[i + 2] - h * h * ff[i]) / (2.0 - h * h * rr[i]);
    }
    for (int i = 0; i < I; i++)
    {
      u[i + 1] = unew[i];
    }

    if (step % 10000 == 0)
    {
      if (p == 0)
      {
        INT n = 1 + globalIndex(L, R, p, I - 1);
        double x = h * n;
        double next = unew[I - 1];
        double real = u_analytic(x);
        printf("%d: x %.6lf, val %.7lf, real val %.7lf, diff %.16lf, tol %.16lf\n", step, x, next, real, next - real, last - next);
        last = next;
      }
    }

    /* RB communication of overlap */
    if (p % 2 == 0)
    { // red phase
      if (p + 1 < P)
      {
        custom_send(unew[I - 1], p, p + 1);
        u[I + 2 - 1] = custom_recieve(p, p + 1);
      }
      if (p - 1 >= 0)
      {
        custom_send(unew[0], p, p - 1);
        u[0] = custom_recieve(p, p - 1);
      }
    }
    else
    { // black phase
      if (p - 1 >= 0)
      {
        u[0] = custom_recieve(p, p - 1);
        custom_send(unew[0], p, p - 1);
      }
      if (p + 1 < P)
      {
        u[I + 2 - 1] = custom_recieve(p, p + 1);
        custom_send(unew[I - 1], p, p + 1);
      }
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);

  if (p - 1 >= 0)
  {
    custom_recieve(p, p - 1);
  }

  /*for (int i = 0; i < I; i++)
  {
    INT n = 1 + globalIndex(L, R, p, i);
    double xn = n * h;
    printf("%d, %d: %.8f %.16f\n", p, i, xn, unew[i]);
  }*/

  char name[100];
  sprintf(name, "./%s.bin", "data");

  FILE *file = p == 0 ? fopen(name, "w") : fopen(name, "a");
  if (file == NULL)
  {
    printf("Error opening %s!\n", name);
    exit(1);
  }

  if (p == 0)
  {
    INT iters = SMX;
    fwrite(&n, sizeof(INT), 1, file);
    fwrite(&iters, sizeof(INT), 1, file);
  }
  fwrite(unew, sizeof(double), I, file);
  fclose(file);

  if (p + 1 < P)
  {
    custom_send(0.0, p, p + 1);
  }
  /* output for graphical representation */
  /* Instead of using gather (which may lead to excessive memory requirements
   on the master process) each process will write its own data portion. This
   introduces a sequentialization: the hard disk can only write (efficiently)
   sequentially. Therefore, we use the following strategy:
   1. The master process writes its portion. (file creation)
   2. The master sends a signal to process 1 to start writing.
   3. Process p waites for the signal from process p-1 to arrive.
   4. Process p writes its portion to disk. (append to file)
   5. process p sends the signal to process p+1 (if it exists).
*/

  printf("DONE %d\n", p);
  MPI_Finalize();
}
