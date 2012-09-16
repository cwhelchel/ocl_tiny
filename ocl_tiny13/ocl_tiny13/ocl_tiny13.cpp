// ocl_tiny13.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "omp_to_ocl.h"

#define N	1024

int	x[N], sync[N], work[N];


void tests (int *x)
{
    int	i;
    int iam;
    int neighbor;

    for (i=0;  i<N;  i++) {
        sync[i] = 0;
        work[i] = -1;
    }

    cl_mem sync_buff = NULL;
    cl_mem x_buff = NULL;
    cl_mem work_buff = NULL;
    
    o2o_init();
    o2o_print_device_info();
    o2o_create_cmd_queue();
    
    sync_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*N, sync);
    x_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*N, x);
    work_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*N, work);

    o2o_open_and_build("tiny13.cl", "tiny13");

    o2o_set_kernel_arg(0, sizeof(cl_mem), &sync_buff); 
    o2o_set_kernel_arg(1, sizeof(cl_mem), &x_buff); 
    o2o_set_kernel_arg(2, sizeof(cl_mem), &work_buff); 

    o2o_execute_kernel(1024); 

    o2o_read_buffer(sync_buff, sizeof(int)*N, sync);
    o2o_read_buffer(x_buff, sizeof(int)*N, x);
    o2o_read_buffer(work_buff, sizeof(int)*N, work);

    o2o_finalize(sync_buff);
    o2o_finalize(x_buff);
    o2o_finalize(work_buff);
}

// this is another tiny test that is a hard fit in OpenCL
// this test is testing the synchronization between Omp
// threads by using memory fences and barriers and the results of
// a parallel groups neighbors
int _tmain(int argc, _TCHAR* argv[])
{
    int	i, thds, v;

    int	errors = 0;

    for (i=0;  i<N;  i++) {
        x[i] = -1;
    }

    tests (x);

    thds = 1024;
    for (i=0;  i<thds;  i++) 
    {
        // this check may be bad b/c we had to modify the kernel to 
        // do something semantically different b/c there is no 
        // omp_get_num_threads opencl call (we can fake it with
        // get_global_size)
        v = (i > 0 ? i : thds) - 1;

        if (x[i] != v) 
        {
            errors += 1;
            printf ("flush 001 - expected x[%d] = %d, observed %d\n",
                i, v, x[i]);
        }
    }

    if (errors == 0) {
        printf ("flush 013 PASSED\n");
    } else {
        printf ("flush 013 FAILED\n");
    }
    return 0;
}

