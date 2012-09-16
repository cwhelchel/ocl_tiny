
void work(int *ip, int *jp)
{
    *ip = 1;
    *jp = 2;
}

__kernel void at001s(__global int* x, __global int* y, __global int* index)
{
	// omp private
    int xlocal;
	int ylocal;

    work(&xlocal, &ylocal);

    //#pragma omp atomic
    //x[index[i]] += xlocal;

    int i = get_global_id(0);
    atomic_add(&x[index[i]], xlocal);

    y[i] += ylocal;
}