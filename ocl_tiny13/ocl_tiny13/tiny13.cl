
__kernel void tiny13(__global int* sync, __global int* x,  __global int* work)
{
    // openmp private vars
    int neighbor;
    int iam;

    iam = get_global_id(0);
    sync[iam] = 0;

    //#pragma omp barrier
    barrier(CLK_GLOBAL_MEM_FENCE);

    /* Do computation into my portion of work array */
    work[iam] = iam;
    //#pragma omp flush

    sync[iam] = 1;
    mem_fence(CLK_GLOBAL_MEM_FENCE); //#pragma omp flush



    neighbor = (iam > 0 ? iam  : get_global_size(0)) - 1;

    while (sync[neighbor] == 0) {
        mem_fence(CLK_GLOBAL_MEM_FENCE); //#pragma omp flush
    }

    /* Read neighbor's values of work array */
    mem_fence(CLK_GLOBAL_MEM_FENCE); //#pragma omp flush
    x[iam] = work[neighbor];
}