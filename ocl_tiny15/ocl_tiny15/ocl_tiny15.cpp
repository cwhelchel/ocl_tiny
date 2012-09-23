// ocl_tiny15.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "omp_to_ocl.h"

#define	N	1024

#define NUM_THREADS 128

int	x[N], y[N];

void test1 ()
{
    int	i;
    int np = NUM_THREADS;
    

    cl_mem x_buff = NULL;
    cl_mem y_buff = NULL;

    o2o_init();
    o2o_print_device_info();
    o2o_create_cmd_queue();

    x_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*N, x);
    y_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*N, y);

    o2o_open_and_build("tiny15.cl", "test1");

    o2o_set_kernel_arg(0, sizeof(cl_mem), &x_buff); 
    o2o_execute_kernel(np); 

    o2o_create_kernel("test2");
    o2o_set_kernel_arg(0, sizeof(cl_mem), &y_buff); 
    o2o_execute_kernel(np); 

    o2o_read_buffer(x_buff, sizeof(int)*N, x);
    o2o_read_buffer(y_buff, sizeof(int)*N, y);

    o2o_finalize(x_buff);
    o2o_finalize(y_buff);
}


void test2 ()
{
    //#pragma omp declaration i
    int	i;


#pragma omp parallel private(i) num_threads( (omp_get_max_threads ()) )
    {
        i = omp_get_thread_num ();
        work2 (i);
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    int	i, thds;

    int	errors = 0;


    for (i=0;  i<N;  i++) {
        x[i] = -1;
        y[i] = -1;
    }

    test1 ();
    test2 ();

    thds = omp_get_max_threads ();
    for (i=0; i<thds; i++) {
        if (x[i] != i) {errors++;
        printf ("omp_get_thread_num 015 - expected x[%d] = %d, observed %d\n",
            i, i, x[i]);
        break;
        }
    }
    for (; i<N; i++) {
        if (x[i] != -1) {errors++;
        printf ("omp_get_thread_num 015 - expected x[%d] = %d, observed %d\n",
            i, -1, x[i]);
        break;
        }
    }
    for (thds=0; thds<N; thds++) {
        if (y[thds] != thds) {
            break;
        }
    }
    for (i=thds; i<N; i++) {
        if (y[i] != -1) {errors++;
        printf ("omp_get_thread_num 015 - expected y[%d] = %d, observed %d\n",
            i, -1, y[i]);
        break;
        }
    }

    printf ("this program run on %d threads\n", thds);

    if (errors == 0) {
        printf ("omp_get_thread_num 015 : PASSED\n");
    } else {
        printf ("omp_get_thread_num 015 : FAILED\n");
    }
    return 0;
}

