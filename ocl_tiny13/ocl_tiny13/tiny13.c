/*
 * OpenMP C API Test Suite
 * Example A.13 from OpenMP C/C++ API sepecification
 */
#include <stdio.h>
#include <omp.h>
#define N	1024


int	x[N], sync[N], work[N];


void tests (
//#pragma omp declaration x
		int *x)
{
	int	i;
//#pragma omp declaration iam
	int iam;
//#pragma omp declaration neighbor
	int neighbor;


  for (i=0;  i<N;  i++) {
    sync[i] = 0;
    work[i] = -1;
  }

#pragma omp parallel private(iam,neighbor) shared(x) num_threads( (omp_get_max_threads ()) )
  {
    iam = omp_get_thread_num ();
    sync[iam] = 0;
#pragma omp barrier

    /* Do computation into my portion of work array */
    work[iam] = iam;
#pragma omp flush

    sync[iam] = 1;
#pragma omp flush
    neighbor = (iam >0 ? iam  : omp_get_num_threads()) - 1;
    while (sync[neighbor] == 0) {
#pragma omp flush
    }
    
    /* Read neighbor's values of work array */
#pragma omp flush
    x[iam] = work[neighbor];
  }
}

int main (int argc, char *argv[])
{
  int	i, thds, v;

  int	errors = 0;

  for (i=0;  i<N;  i++) {
    x[i] = -1;
  }

  tests (x);

  thds = omp_get_max_threads ();
  for (i=0;  i<thds;  i++) {
    v = (i > 0 ? i : thds) - 1;
    if (x[i] != v) {
      errors += 1;
      printf ("flush 001 - expected x[%d] = %d, observed %d\n",
	      i, v, x[i]);
    }
  }

  if (errors == 0) {
    printf ("flush 013 PASSED\n");
  } else {
    printf ("flush 013 FAILED\n");
  }
  return 0;
}

