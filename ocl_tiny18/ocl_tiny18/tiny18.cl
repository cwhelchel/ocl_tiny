#pragma OPENCL EXTENSION cl_amd_printf : enable

void work(__global int* iw, int i, int j)
{
	int width = get_global_size(0);
    iw[(i * width) + j] = i + j * 117;
}

__kernel void d003s(__global int* data)
{
    int i = get_global_id(0);
    int j = get_global_id(1);

	printf("i %i j %i \n", i, j);
	work(data, i, j);
}
