// ocl_tiny03.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "omp_to_ocl.h"

#define NUM 1024

int  x[NUM+1];

//void subdomain(int *x, int iam, int ipoints)
//{
//    int  i, i1, i2;
//
//    i1 = iam * ipoints + 1;
//    i2 = i1 + ipoints - 1;
//    for( i = i1 ; i <= i2 ; i++ )
//      x[i] = iam;
//}

void par001s(int *x, int npoints)
{
    //int  iam;
	//int np;
	//int ipoints;

//#pragma omp parallel shared(x, npoints) private(iam, np, ipoints)
    //{
	//iam = omp_get_thread_num();
	//np = omp_get_num_threads();
	//ipoints = npoints / np;
	//subdomain(x, iam, ipoints);
    //}

    cl_mem x_buff = NULL;
    cl_mem npoints_buff = NULL;
    
    o2o_init();
    o2o_create_cmd_queue();
    
    x_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*NUM+1, x);
    npoints_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int), &npoints);

    o2o_open_and_build("tiny03.cl", "par001s");

    o2o_set_kernel_arg(0, sizeof(cl_mem), &x_buff); 
    o2o_set_kernel_arg(1, sizeof(cl_mem), &npoints_buff); 

    o2o_execute_kernel(NUM);

    o2o_read_buffer(x_buff, sizeof(int)*NUM+1, x);

    o2o_finalize(x_buff);
    o2o_finalize(npoints_buff);


    //for (int i =0; i<NUM+1; i++)
    //    printf("x[i] is %i\n", x[i]);
}

extern "C" int tiny03(int argc, char **argv);

int _tmain(int argc, _TCHAR* argv[])
{
    const int  n = NUM;
    int  i, errors, ilast, nt;

    for( i = 1 ; i <= n ; i++ )
	x[i] = -1;

    par001s(x, n);
    //tiny03(argc, NULL);

	/* Determine last element modified */
    ilast = 0;
    for( i = 1 ; i <= n ; i++ ){
	if ( x[i] < 0 )
	    break;
	ilast = i;
    }
	/* Infer number of threads */
    nt = x[ilast] + 1;
    errors = 0;
	/* Should be fewer than NT points not modified */
    if ( n - ilast > nt ){
	errors += 1;
	printf("parallel - Threads do not divide points changed\n");
    }
    printf("parallel - Apparent number threads = %d\n", nt);
    if ( errors == 0 )
	printf("parallel 003 PASSED\n");
    else{
	printf("parallel -   Number points = %d\n", n);
	printf("parallel -   Points changed = %d\n", ilast);
	printf("parallel 003 FAILED\n");
    }
	return 0;
}

