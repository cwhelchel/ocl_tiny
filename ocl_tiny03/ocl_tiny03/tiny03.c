/*
 * OpenMP C API Test Suite
 * Example A.3 from OpenMP C/C++ API sepecification
 */

#include "stdafx.h"
#include <stdio.h>
#include <omp.h>

#define NUM 1024


int  x[NUM+1];


void subdomain(int *x, int iam, int ipoints)
{
    int  i, i1, i2;

    i1 = iam * ipoints + 1;
    i2 = i1 + ipoints - 1;
    for( i = i1 ; i <= i2 ; i++ )
	x[i] = iam;
}

void par001s(
//#pragma omp declaration x
int *x,
//#pragma omp declaration npoints
int npoints)
{
//#pragma omp declaration iam
    int  iam;
//#pragma omp declaration np
	int np;
//#pragma omp declaration ipoints
	int ipoints;

#pragma omp parallel shared(x, npoints) private(iam, np, ipoints)
    {
	iam = omp_get_thread_num();
	np = omp_get_num_threads();
	ipoints = npoints / np;
	subdomain(x, iam, ipoints);
    }
}

int tiny03(int argc, char **argv)
{

    const int  n = NUM;
    int  i, errors, ilast, nt;

    for( i = 1 ; i <= n ; i++ )
	x[i] = -1;

    par001s(x, n);
	/* Determine last element modified */
    ilast = 0;
    for( i = 1 ; i <= n ; i++ ){
	if ( x[i] < 0 )
	    break;
	ilast = i;
    }
	/* Infer number of threads */
    nt = x[ilast] + 1;
    errors = 0;
	/* Should be fewer than NT points not modified */
    if ( n - ilast > nt ){
	errors += 1;
	printf("parallel - Threads do not divide points changed\n");
    }
    printf("parallel - Apparent number threads = %d\n", nt);
    if ( errors == 0 )
	printf("parallel 003 PASSED\n");
    else{
	printf("parallel -   Number points = %d\n", n);
	printf("parallel -   Points changed = %d\n", ilast);
	printf("parallel 003 FAILED\n");
    }
	return 0;
}
