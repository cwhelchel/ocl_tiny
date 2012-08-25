// ocl_tiny04.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include "omp_to_ocl.h"

#define M	117
#define N	511

int  a[N], b[N], y[M], z[M];


void d001s(
    int m,
    int n,
    int *a,
    int *b,
    int *y,
    int *z)
{
    cl_mem a_buff = NULL;
    cl_mem b_buff = NULL;
    cl_mem y_buff = NULL;
    cl_mem z_buff = NULL;

    o2o_init();
    o2o_create_cmd_queue();

    a_buff = o2o_create_buffer(CL_MEM_READ_ONLY,  sizeof(int)*N, a);
    b_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*N, b);
    y_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*M, y);
    z_buff = o2o_create_buffer(CL_MEM_READ_ONLY,  sizeof(int)*M, z);

    // first kernel from tiny04
    o2o_open_and_build("tiny04.cl", "d001s_for1");

    o2o_set_kernel_arg(0, sizeof(cl_mem), &a_buff); 
    o2o_set_kernel_arg(1, sizeof(cl_mem), &b_buff); 

    o2o_execute_kernel(N);

    o2o_read_buffer(b_buff, sizeof(int)*N, b);

    // second kernel from tiny04
    o2o_open_and_build("tiny04.cl", "d001s_for2");

    o2o_set_kernel_arg(0, sizeof(cl_mem), &y_buff); 
    o2o_set_kernel_arg(1, sizeof(cl_mem), &z_buff); 

    o2o_execute_kernel(M);

    o2o_read_buffer(y_buff, sizeof(int)*M, y);

    o2o_finalize(a_buff);
    o2o_finalize(b_buff);
    o2o_finalize(y_buff);
    o2o_finalize(z_buff);
}



int _tmain(int argc, _TCHAR* argv[])
{
    int  m, n, i, errors;

    m = M;
    n = N;
    for( i = 0 ; i < n ; i++ ){
        a[i] = 2*i + 1;
        b[i] = 0;
    }
    for( i = 0 ; i < m ; i++ ){
        z[i] = i*i;
        y[i] = 0;
    }

    d001s(m, n, a, b, y, z);

    errors = 0;

    for( i = 1 ; i < n ; i++ ) 
    {
        if ( b[i] != 2*i )
        {
            errors += 1;
            if ( errors == 1 )
                printf("for - VALUES IN B ARE NOT AS EXPECTED\n");

            printf("EXPECTED B[%d] = %d OBSERVED %d\n", i, 2*i, b[i]);
        }
    }

    for( i = 1 ; i < m ; i++ )
    {
        if ( y[i] != i )
        {
            errors += 1;
            if ( errors == 1 )
                printf("for - VALUES IN Y ARE NOT AS EXPECTED\n");

            printf("EXPECTED Y[%d] = %d OBSERVED %d\n", i, i, y[i]);
        }
    }

    if ( errors == 0 )
        printf("for 004 PASSED\n");
    else
        printf("for 004 FAILED\n");
    return 0;
}

