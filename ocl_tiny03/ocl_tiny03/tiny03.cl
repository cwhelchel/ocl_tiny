void subdomain(__global int *x, int iam, int ipoints)
{
    int  i, i1, i2;

    i1 = iam * ipoints + 1;
    i2 = i1 + ipoints - 1;

    for( i = i1 ; i <= i2 ; i++ )
        x[i] = iam;
}

// here we can put npoints in the constant address space since
// it is just the total number of points and won't change.
__kernel void par001s(__global int* x, __constant int* npoints)
{
    int iam = get_global_id(0);
	int np = get_global_size(0);

	int ipoints = (*npoints) / np;
	
    subdomain(x, iam, ipoints);
}