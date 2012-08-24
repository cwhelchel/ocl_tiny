/*
 * OpenMP C API Test Suite
 * Example A.12 from OpenMP C/C++ API sepecification
 */
#include <stdio.h>
#define  N	613

int  x[2], y[N], indx[N];


void work(int *ip, int *jp)
{
    *ip = 1;
    *jp = 2;
}

void at001s(
#pragma omp declaration n
			int n,
#pragma omp declaration x
			int *x,
#pragma omp declaration y
			int *y,
#pragma omp declaration index
			int *index)
{
    int  i;
#pragma omp declaration xlocal
	int xlocal;
#pragma omp declaration ylocal
	int ylocal;

#pragma omp parallel for private(xlocal,ylocal) shared(x, y, index, n)
    for ( i = 0 ; i < n ; i++ ) {
		work(&xlocal, &ylocal);
#pragma omp atomic
		x[index[i]] += xlocal;
		y[i] += ylocal;
    }
}

int main(int argc, char *argv[])
{
    int  i, errors, errors1, n;

    n = N;
    x[0] = 0;
    x[1] = 0;
    for( i = 0 ; i < n ; i++ ){
	y[i] = i;
	if ( i < n / 2 )
	    indx[i] = 0;
	else
	    indx[i] = 1;
    }
    at001s(n, x, y, indx);

    errors = 0;
    if ( x[0] != n/2 ){
	errors += 1;
	printf("atomic 001 - EXPECTED X[%d] = %d OBSERVED %d\n", 0, n/2, x[0]);
    }
    if ( x[1] != n - n/2 ){
	errors += 1;
	printf("atomic 001 - EXPECTED X[%d] = %d OBSERVED %d\n", 1, n-n/2, x[1]);
    }
    for( i = 0 ; i < n / 2 ; i++ ){
	if ( y[i] != 2+i )
	    printf("atomic 001 - EXPECTED Y[%d] = %d OBSERVED %d\n", i, 2+i, y[i]);
    }

    if ( errors == 0 )
	printf("atomic 012 PASSED\n");
    else
	printf("atomic 012 FAILED\n");
	return 0;
}
