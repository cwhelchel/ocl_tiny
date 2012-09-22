void work (int i)
{
    x[i] = i;
}


void work2 (int i)
{
    y[i] = i;
}


__kernel void test1 (__global int* np, __global int* x, __global int* y)
{
    int	i = get_global_id(0);
    for (i=0;  i<np;  i++) {
        work (i);
    }
}


__kernel void test2 ()
{
    int i = get_global_id(0);
    work2 (i);
}
