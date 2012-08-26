// ocl_tiny07.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "omp_to_ocl.h"
#include <stdio.h>


#define  N	1024


void pd002s(int  n, int*a, int *b)
{
    int x[N] = {0};
    int y[N] = {0};
    cl_mem x_buff = NULL;
    cl_mem y_buff = NULL;
    
    o2o_init();
    o2o_create_cmd_queue();
    
    x_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*N, &x);
    y_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*N, &y);

    o2o_open_and_build("tiny07.cl", "pd002s");

    o2o_set_kernel_arg(0, sizeof(cl_mem), &x_buff); 
    o2o_set_kernel_arg(1, sizeof(cl_mem), &y_buff); 

    o2o_execute_kernel(n);

    o2o_read_buffer(x_buff, sizeof(int)*N, &x);
    o2o_read_buffer(y_buff, sizeof(int)*N, &y);

    o2o_finalize(x_buff);
    o2o_finalize(y_buff);

    // reduce all the things
    int x_red=0, y_red=0;
    for (int i=0;i<N; i++)
    {
        x_red += x[i];
        y_red += y[i];
    }

    *a += x_red;
    *b += y_red;
}

int _tmain(int argc, _TCHAR* argv[])
{
    int  i, n, errors;
    int  a, b;

    n = N;
    a = 0;
    b = n;

    pd002s(n, &a, &b);

    errors = 0;
    if ( a != n ){
        errors += 1;
        printf("parallel for 002 - EXPECTED A = %d OBSERVED %d\n", n, a);
    }
    if ( b != 0 ){
        errors += 1;
        printf("parallel for 002 - EXPECTED B = %d OBSERVED %d\n", 0, b);
    }

    if ( errors == 0 )
        printf("parallel for 007 PASSED\n");
    else
        printf("parallel for 007 FAILED\n");
    return 0;
}

