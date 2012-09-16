// ocl_tiny12.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include "omp_to_ocl.h"

#define  N	613

int  x[2], y[N], indx[N];

void at001s(int n, int *x, int *y, int *index)
{
    cl_mem x_buff = NULL;
    cl_mem y_buff = NULL;
    cl_mem index_buff = NULL;
    
    o2o_init();
    o2o_print_device_info();
    o2o_create_cmd_queue();
    
    x_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*2, x);
    y_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*N, y);
    index_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*N, index);

    o2o_open_and_build("tiny12.cl", "at001s");

    o2o_set_kernel_arg(0, sizeof(cl_mem), &x_buff); 
    o2o_set_kernel_arg(1, sizeof(cl_mem), &y_buff); 
    o2o_set_kernel_arg(2, sizeof(cl_mem), &index_buff);

    o2o_execute_kernel(N); 

    o2o_read_buffer(x_buff, sizeof(int)*2, x);
    o2o_read_buffer(y_buff, sizeof(int)*N, y);

    o2o_finalize(x_buff);
    o2o_finalize(y_buff);
    o2o_finalize(index_buff);
}

int _tmain(int argc, _TCHAR* argv[])
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

