#pragma OPENCL EXTENSION cl_amd_printf : enable

void work(ip, jp)
    int  *ip, *jp;
{
    *ip = 1;
    *jp = -1;
}

__kernel void pd002s(__global int* x_red, __global int* y_red)
{
    int alocal = 0, blocal = 0;
    int x = 0, y = 0;

    work(&alocal, &blocal);
    x += alocal;
    y += blocal;

    x_red[get_global_id(0)] = x;
    y_red[get_global_id(0)] = y;


    //barrier(CLK_GLOBAL_MEM_FENCE);

    
    
   
}