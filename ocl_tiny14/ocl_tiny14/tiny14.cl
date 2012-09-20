
void f1(int *q)
{
    *q = 1;
    
    mem_fence(CLK_GLOBAL_MEM_FENCE);

    //#pragma omp flush
    /* x, p, and *q are flushed
    * because they are shared and accessible 
    */
}


void f2(int *q)
{
    *q = 2;

    barrier(CLK_GLOBAL_MEM_FENCE);

    //#pragma omp barrier
    /* a barrier implies a flush
    * x, p, and *q are flushed
    * because they are shared and accessible
    */
}

//#pragma omp parallel reduction(+:sum) num_threads( (omp_get_max_threads ()) ) private(j) shared(i,n)
__kernel void tiny14(__global int* i, __global int* n, __global int* sum_red)
{
    int sum_red_idx = get_global_id(0);

    int j;

    f1(&j);

    /* 
      i and n were not flushed
      because they were not accessible in f1
      j was flushed because it was accessible
    */

    sum_red[sum_red_idx] += j;

    //#pragma omp barrier
    barrier(CLK_GLOBAL_MEM_FENCE);

    f2(&j);

    /* i and n were not flushed
    * because they were not acessible in f2
    * j was flushed because it was accessible
    */

    sum_red[sum_red_idx] += *i + j + 1 + *n; //*p was always 1
}