/*
 * OpenMP C API Test Suite
 * Example A.1 from OpenMP C/C++ API sepecification
 */

#include <stdio.h>

#define  N	1024

int  a[N], b[N];


void pd001s(
// [cmw] omp declaration is not a valid OpenMP directive or clause
//#pragma omp declaration n,a,b
int n,
int *a,
int *b)
{
#pragma omp parallel shared(a,b,n)
{
    int  i;
#pragma omp for
    for( i = 1 ; i < n ; i++ ) {
	b[i] = (a[i] + a[i-1]) / 2;
    }
}
}


int tiny01(int argc, char **argv)
{
    int  i, n, errors;

    n = N;
    for( i = 0 ; i < n ; i++ ){
	a[i] = 2*i + 1;
	b[i] = 0;
    }

    pd001s(n, a, b);

    errors = 0;
    for( i = 1 ; i < n ; i++ ){
	if ( b[i] != 2*i ){
	    errors += 1;
	    if ( errors == 1 )
		printf("parallel for - VALUES IN B ARE NOT AS EXPECTED\n");
	    printf("EXPECTED B[%d] = %d OBSERVED %d\n", i, 2*i, b[i]);
	}
    }

    if ( errors == 0 )
	printf("parallel for 001 PASSED\n");
    else
	printf("parallel for 001 FAILED\n");
    return 0;
}
