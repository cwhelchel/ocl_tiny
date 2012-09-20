// ocl_tiny14.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "omp_to_ocl.h"

int	x;
int	*p;

void test(int n, int i, int work_size, int* sum)
{
    cl_mem i_buff = NULL;
    cl_mem n_buff = NULL;
    cl_mem sum_red_buff = NULL;

    int* sum_red = (int*) malloc(work_size * sizeof(int));
    memset(sum_red, 0, work_size * sizeof(int));

    o2o_init();
    o2o_print_device_info();
    o2o_create_cmd_queue();

    i_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int), &i);
    n_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int), &n);
    sum_red_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*work_size, sum_red);

    o2o_open_and_build("tiny14.cl", "tiny14");

    o2o_set_kernel_arg(0, sizeof(cl_mem), &i_buff); 
    o2o_set_kernel_arg(1, sizeof(cl_mem), &n_buff); 
    o2o_set_kernel_arg(2, sizeof(cl_mem), &sum_red_buff); 

    o2o_execute_kernel(work_size); 

    //o2o_read_buffer(i_buff, sizeof(int)*N, sync);
    //o2o_read_buffer(n_buff, sizeof(int)*N, x);
    o2o_read_buffer(sum_red_buff, sizeof(int)*work_size, sum_red);

    // reduction op: +
    for (int idx = 0; idx < work_size; idx++)
        *sum += sum_red[idx];

    o2o_finalize(i_buff);
    o2o_finalize(n_buff);
    o2o_finalize(sum_red_buff);

    free(sum_red);
}

int _tmain(int argc, _TCHAR* argv[])
{
    int ret, thds = 20;
    int errors = 0;
    int sum;
    p = &x;

    sum = 0;
    test(10, 1, thds, &sum);
    ret = sum;

    thds = thds;
    if (ret != 15*thds) {
        errors += 1;
        printf ("flush 014 - expected sum = %d, observed %d\n",
            ret, 15 * thds);
    }

  if (errors == 0) {
    printf ("flush 014 PASSED\n");
  } else {
    printf ("flush 014 FAILED\n");
  }
    return 0;
}

