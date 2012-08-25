// ocl_tiny05.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
    // tiny05 omp may not be translatable without a large
    // re-write of the programin in ocl. the omp algorithm
    // relies on the omp critical section which ocl doesn't have.
	return 0;
}

