// ocl_tiny01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include "omp_to_ocl.h"

#define  N	1024

int  a[N], b[N];

void pd001s(int n,int *a,int *b)
{
    cl_mem a_buff = NULL;
    cl_mem b_buff = NULL;
    
    o2o_init();
    o2o_create_cmd_queue();
    
    a_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*N, a);
    b_buff = o2o_create_buffer(CL_MEM_WRITE_ONLY, sizeof(int)*N);

    o2o_open_and_build("pd001s.cl", "pd001s");

    o2o_set_kernel_arg(0, sizeof(cl_mem), &a_buff); 
    o2o_set_kernel_arg(1, sizeof(cl_mem), &b_buff); 

    o2o_execute_kernel(N);

    o2o_read_buffer(b_buff, sizeof(int)*N, b);

    o2o_finalize(a_buff);
    o2o_finalize(b_buff);
}


int _tmain(int argc, _TCHAR* argv[])
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

