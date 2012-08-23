

__kernel void pd001s(__global int* a, __global int* b)
{
    int i = get_global_id(0);

    b[i] = (a[i] + a[i-1]) / 2;
}