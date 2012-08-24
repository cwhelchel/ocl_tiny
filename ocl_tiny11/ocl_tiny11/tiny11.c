/*
 * OpenMP C API Test Suite
 * Example A.11 from OpenMP C/C++ API sepecification
 */
#include <stdio.h>
#define N	1024


int x[N];


void
do_by_x (x, iam, ipoints)
     int	x[];
     int	iam, ipoints;
{
  int	i;


  for (i = 0;  i < ipoints;  i ++) {
    x[iam*ipoints + i] = iam;
  }
}



void
test (
#pragma omp declaration x
	  int *x,
#pragma omp declaration npoints
	  int npoints,
#pragma omp declaration thds
	  int thds)
{
#pragma omp declaration iam
	int	iam;
#pragma omp declaration ipoints
	int ipoints;


  printf ("set thread = %d\n", thds);

  omp_set_dynamic (0);
  omp_set_num_threads (thds);
#pragma omp parallel shared (x,npoints,thds) private (iam, ipoints)
  {
    if (omp_get_num_threads () != thds)
      abort ();

    iam = omp_get_thread_num ();
    ipoints = npoints/thds;
    do_by_x(x, iam, ipoints);
  }
}



int
checkx (int x[], int npoints, int thds)
{
  int	i, j, ipoints;

  int	errors = 0;


  ipoints = npoints / thds;
  for (i = 0;  i < thds;  i ++) {
    for (j = 0  ;  j < ipoints;  j ++) {
      if (x[i * ipoints + j] != i) {
	errors += 1;
	printf ("omp_set_num_threads - expected x[%d] = %d, observed %d\n",
		i * ipoints + j, i, x[i * ipoints + j]);
      }
    }
  }

  for (i = thds * ipoints;  i < npoints;  i ++) {
    if (x[i] != -1) {
      errors += 1;
      printf ("omp_set_num_threads - expected x[%d] = %d, observed %d\n",
	      i, -1, x[i]);
    }
  }

  return errors;
}



int main (int argc, char *argv[])
{
  int	i, j, thds;

  int	errors = 0;

  thds = omp_get_max_threads ();
  for (i = 1;  i <= thds;  i ++) {
    for (j = 0;  j < N;  j ++) {
      x[j] = -1;
    }

    test (x, N, i);
    errors += checkx (x, N, i);
  }

  if (errors == 0) {
    printf ("omp_set_num_threads 011 PASSED\n");
  } else {
    printf ("omp_set_num_threads 011 FAILED\n");
  }
  return 0;
}
