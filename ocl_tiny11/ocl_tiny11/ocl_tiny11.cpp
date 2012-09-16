// ocl_tiny11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "omp_to_ocl.h"

#define N	1024


int x[N];

void  test (int *x,    int npoints,    int thds)
{
    /*int	iam;
    int ipoints;

    printf ("set thread = %d\n", thds);

    omp_set_dynamic (0);
    omp_set_num_threads (thds);*/

    cl_mem x_buff = NULL;
    cl_mem n_buff = NULL;
    cl_mem thds_buff = NULL;
    
    o2o_init();
    o2o_print_device_info();
    o2o_create_cmd_queue();
    int nVal = N;
    
    x_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*N, x);
    n_buff = o2o_create_buffer(CL_MEM_READ_ONLY, sizeof(int), &nVal);
    thds_buff = o2o_create_buffer(CL_MEM_READ_ONLY, sizeof(int), &thds);

    o2o_open_and_build("tiny11.cl", "test");

    o2o_set_kernel_arg(0, sizeof(cl_mem), &x_buff); 
    // we don't really need the N kernel arg as we could
    // just read global size in the kernel
    o2o_set_kernel_arg(1, sizeof(cl_mem), &n_buff); 
    o2o_set_kernel_arg(2, sizeof(cl_mem), &thds_buff); 

    o2o_execute_kernel(1024); 

    o2o_read_buffer(x_buff, sizeof(int)*N, x);
    

    o2o_finalize(x_buff);
    o2o_finalize(n_buff);
    o2o_finalize(thds_buff);

}



int
    checkx (int x[], int npoints, int thds)
{
    int	i, j, ipoints;

    int	errors = 0;


    ipoints = npoints / thds;
    for (i = 0;  i < thds;  i ++) {
        for (j = 0  ;  j < ipoints;  j ++) {
            if (x[i * ipoints + j] != i) {
                errors += 1;
                printf ("omp_set_num_threads - expected x[%d] = %d, observed %d\n",
                    i * ipoints + j, i, x[i * ipoints + j]);
            }
        }
    }

    for (i = thds * ipoints;  i < npoints;  i ++) {
        if (x[i] != -1) {
            errors += 1;
            printf ("omp_set_num_threads - expected x[%d] = %d, observed %d\n",
                i, -1, x[i]);
        }
    }

    return errors;
}

// the tiny11 test might not be worthwhile to translate as the omp test
// seems to be checking results based off of the max number of threads
// allowed.
int _tmain(int argc, _TCHAR* argv[])
{
    int	i, j, thds;

    int	errors = 0;

    thds = 2; // arbitray max thread count

    for (i = 1;  i <= thds;  i ++) 
    {
        for (j = 0;  j < N;  j ++) 
        {
            x[j] = -1;
        }

        test (x, N, i);
        errors += checkx (x, N, i);
    }

    if (errors == 0) {
        printf ("omp_set_num_threads 011 PASSED\n");
    } else {
        printf ("omp_set_num_threads 011 FAILED\n");
    }

    getchar();
    return 0;
}

