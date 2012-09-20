/*
 * OpenMP C API Test Suite
 * Example A.14 from OpenMP C/C++ API sepecification
 */
#include <stdio.h>
#include <omp.h>

int	x;
int	*p;


void
f1(q)
     int	*q;
{
  *q = 1;
#pragma omp flush
  /* x, p, and *q are flushed
   * because they are shared and accessible 
   */
}


void
f2(q)
     int	*q;
{
  *q = 2;
#pragma omp barrier
  /* a barrier implies a flush
   * x, p, and *q are flushed
   * because they are shared and accessible
   */
}


int
g(
//#pragma omp declaration n
  int n)
{
//#pragma omp declaration j
  int	j;
//#pragma omp declaration i
	int	i;
//#pragma omp declaration sum
	int sum;


	*p = 1; sum=0; i=1;
#pragma omp parallel reduction(+:sum) num_threads( (omp_get_max_threads ()) ) private(j) shared(i,n)
  {
    f1 (&j);
    /* i and n were not flushed
     * because they were not accessible in f1
     * j was flushed because it was accessible
     */
    sum += j;

#pragma omp barrier
    f2(&j);
    /* i and n were not flushed
     * because they were not acessible in f2
     * j was flushed because it was accessible
     */
    sum += i + j + *p + n;
  }
  return sum;
}


int tiny14 (int argc, char *argv[])
{
  int	ret, thds;

  int	errors = 0;

  p = &x;

  ret = g (10);

  thds = omp_get_max_threads ();
  if (ret != 15*thds) {
    errors += 1;
    printf ("flush 014 - expected sum = %d, observed %d\n",
	    ret, 15 * thds);
  }

  if (errors == 0) {
    printf ("flush 014 PASSED\n");
  } else {
    printf ("flush 014 FAILED\n");
  }
  return 0;
}

