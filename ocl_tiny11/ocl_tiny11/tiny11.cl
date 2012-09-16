#pragma OPENCL EXTENSION cl_amd_printf : enable

void do_by_x(__global int* x, int iam, int ipoints)
{
    int	i;
    
    // this won't work because this will be called for
    for (i = 0;  i < ipoints;  i ++) {
        x[iam*ipoints + i] = iam;
    }
}

__kernel void test(__global int *x, __global int* npoints, __global int* thds)
{
    // this check won't work
    //if (get_global_size(0) != *thds)
        //return;

    int iam = get_global_id(0);
    int ipoints = (*npoints)/(*thds);

    printf("%i  %i\n", iam, ipoints);
    //do_by_x(x, iam, ipoints);
}