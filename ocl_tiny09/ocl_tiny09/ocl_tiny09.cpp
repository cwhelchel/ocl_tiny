// ocl_tiny09.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "omp_to_ocl.h"


void sngl001s(int *x, int *y)
{
    cl_mem x_buff = NULL;
    cl_mem y_buff = NULL;
    
    o2o_init();
    o2o_print_device_info();
    o2o_create_cmd_queue();
    
    x_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int), x);
    y_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int), y);

    o2o_open_and_build("tiny09.cl", "sngl001s");

    o2o_set_kernel_arg(0, sizeof(cl_mem), &x_buff); 
    o2o_set_kernel_arg(1, sizeof(cl_mem), &y_buff); 

    // global size here is arbitrary really...
    // OMP did 2 threads - which worked here
    // Tried 1024 which gave one error from validation below
    // Tried 512 and it worked
    // Tried 256 and it worked
    o2o_execute_kernel(512); 

    o2o_read_buffer(x_buff, sizeof(int), x);
    o2o_read_buffer(y_buff, sizeof(int), y);

    o2o_finalize(x_buff);
    o2o_finalize(y_buff);
}


int _tmain(int argc, _TCHAR* argv[])
{
    int  i, errors;

    int x = 0;
    int y = -100;

    sngl001s(&x, &y);

    errors = 0;
    if ( x <= 0 ){
        errors += 1;
        printf("single 001 - Expect positive value, observe X = %d\n", x);
    }
    if ( y <= 0 ){
        errors += 1;
        printf("single 001 - Expect positive value, observe Y = %d\n", y);
    }
    else
        printf("single 001 - Apparent number of threads, Y = %d\n", y);

    if ( x - y != 10 ){
        errors += 1;
        printf("single 001 - Expect difference of 10, observe X = %d, Y = %d\n",
            x, y);
    }

    if ( errors == 0 )
        printf("single 009 PASSED\n");
    else
        printf("single 009 FAILED\n");
    return 0;
}

