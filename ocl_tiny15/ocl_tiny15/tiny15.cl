void work (int i, __global int* x)
{
    x[i] = i;
}


void work2 (int i, __global int* y)
{
    y[i] = i;
}


__kernel void test1(__global int* x)
{
    int	i = get_global_id(0);
    work (i, x);
}


__kernel void test2( __global int* y)
{
    int i = get_global_id(0);
    work2 (i, y);
}
