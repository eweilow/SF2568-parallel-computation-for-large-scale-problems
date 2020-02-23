/* Reaction-diffusion equation in 1D
 * Solution by Jacobi iteration
 * simple MPI implementation
 *
 * C Michael Hanke 2006-12-12
 */

#define INT int64_t

#define MIN(a,b) ((a) < (b) ? (a) : (b))

/* Use MPI */
#include "mpi.h"

/* define problem to be solved */
#define N 100   /* number of inner grid points */
#define SMX 100000 /* number of iterations */

/* implement coefficient functions */
extern double r(const double x){
  return x*x;
}

extern double f(const double x){
  return 0;
}

/* We assume linear data distribution. The formulae according to the lecture
   are:
      L = N/P;
      R = N%P;
      I = (N+P-p-1)/P;    (number of local elements)
      n = p*L+MIN(p,R)+i; (global index for given (p,i)
   Attention: We use a small trick for introducing the boundary conditions:
      - The first ghost point on p = 0 holds u(0);
      - the last ghost point on p = P-1 holds u(1).
   Hence, all local vectors hold I elements while u has I+2 elements.
*/
#define A 2

INT* mu(INT m) {

}

INT* muInverse(INT p, INT i, int P){

   return (L − A)*p + MIN(R, p) + i;
}

int recieve(INT p){
  MPI_Recieve(&u0,
  1,
  MPI_DOUBLE,
  p-1,
  0,   // TAG
  MPI_COMM_WORLD,
  MPI_STATUS_IGNORE);
  return u0;
}

void send(int* data, INT p) {
    data,
    1,
    MPI_DOUBLE,
    p+1,
    0, // TAG
    MPI_COMM_WORLD)
}

int main(int argc, char *argv[])
{
/* local variable */

/* Initialize MPI */
  int P, p, M, L, R, I;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &P);
  MPI_Comm_rank(MPI_COMM_WORLD, &p);
  if (N < P) {
     fprintf(stdout, "Too few discretization points...\n");
     exit(1);
  }
  /* Compute local indices for data distribution */
  M = N + (P-1)*A;
  L = M/P;
  R = R % P;
  I = p < R ? L+1 : L;

/* arrays */
  unew = (double *) malloc(I*sizeof(double));
/* Note: The following allocation includes additionally:
   - boundary conditins are set to zero
   - the initial guess is set to zero */
  u = (double *) calloc(I+2, sizeof(double));


/* Jacobi iteration */
  for (step = 0; step < SMX; step++) {

  /* RB communication of overlap */
    if (p == 0) {

    } else {

      u[0] = recieve(p);
    }
    /* local iteration step */
    for(int i=0; i<I; i++) {
    	    unew[i] = (u[i]+u[i+2]-h*h*ff[i])/(2.0-h*h*rr[i]);
    }
    if (p == P-1) {

    } else {
      send(u[I-1], p);
    }
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

/* That's it */
  MPI_Finalize();
  exit(0);
}
