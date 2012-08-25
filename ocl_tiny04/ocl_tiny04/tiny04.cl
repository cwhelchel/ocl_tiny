int sqrt_func(int k)
{
    int  i, j;

    j = 1;
    for( i = 0 ; i < k ; i++ ){
	if ( j*j != k )
	    j = (j + k/j) / 2;
	else
	    break;
    }
    return (j);
}

__kernel void d001s_for1(__global int* a,
                         __global int* b)
{
    int i = get_global_id(0);

	b[i] = (a[i] + a[i-1]) / 2;
}

__kernel void d001s_for2(__global int* y,
                         __global int* z)
{
    int i = get_global_id(0);
    
	y[i] = sqrt_func(z[i]);
}