// ocl_tiny18.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "omp_to_ocl.h"

#define  N	117

int  iw[N][N];

void init()
{
    int  i, j;

    for( i = 0 ; i < N ; i++ )
        for( j = 0 ; j < N ; j++ )
            iw[i][j] = 0;
}

void d003s(int n)
{
    int	i;
    size_t np[2] = { N, N };

    cl_mem iw_buff = NULL;

    o2o_init();
    o2o_create_cmd_queue();

    iw_buff = o2o_create_buffer(CL_MEM_READ_WRITE, sizeof(int)*N*N, iw);

    o2o_open_and_build("tiny18.cl", "d003s");

    o2o_set_kernel_arg(0, sizeof(cl_mem), &iw_buff); 

    o2o_execute_kernel(np); 


    o2o_read_buffer(iw_buff, sizeof(int)*N*N, iw);

    o2o_finalize(iw_buff);
}

int icheck(char  * ch)
{
    int  i, j, iexpect, res = 0;

    for( i = 0 ; i < N ; i++ )
        for( j = 0 ; j < N ; j++ ){
            iexpect = i + j * N;
            if ( iw[i][j] != iexpect ){
                res += 1;
                printf("for 003 - EXPECTED IW[%d][%d] = %d OBSERVED %d %s\n",
                    i, j, iexpect, iw[i][j], ch);
            }
        }

    return res;
}

int _tmain(int argc, _TCHAR* argv[])
{
    int  i, n, errors;

    n = N;
    errors = 0;
    init();
    d003s(n);
    errors += icheck("after d003s");

    /*init();
    d003t(n);
    errors += icheck("after d003t");*/

    if ( errors == 0 )
        printf("for 018 PASSED\n");
    else
        printf("for 018 FAILED\n");
    return 0;
}

