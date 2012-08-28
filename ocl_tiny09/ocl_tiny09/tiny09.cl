void output(__global int  *x)
{
    *x += 10;
}

void input(__global int  *y)
{
    *y += 100;
}

void work(__global int  *x)
{
    //#pragma omp atomic
    //*x += 1;
    atomic_inc(x);
}

__kernel
void sngl001s(__global int *x, __global int* y)
{
    work(x);
    barrier(CLK_GLOBAL_MEM_FENCE);

    if (get_global_id(0) == 0)
    {
        output(x);
        input(y);
    }

    barrier(CLK_GLOBAL_MEM_FENCE);

    work(y);
}
