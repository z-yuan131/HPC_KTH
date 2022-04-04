#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double mysecond(){
  struct timeval tp;
  struct timezone tzp;
  int i;

  i = gettimeofday(&tp,&tzp);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

/*
 * Example of sparse matrix-vector multiply, using CSR (compressed sparse
 * row format). 
 *
 */
int main(int argc, char *argv[])
{
    int    *ia, *ja;
    double *a, *x, *y;
    int    row, i, j, idx, n, nnzMax, nnz, nrows;
    double ts, t, rate, flops, bandwidth;

    n = 10000;
    if (argc > 1) n = atoi(argv[1]);

    /* Warning: if n > sqrt(2^31), you may get integer overflow */

    /* Allocate enough storage for the matix.  We allocate more than
       is needed in order to simplify the code */

    nrows  = n * n;
    nnzMax = nrows * 5;
    ia = (int*)malloc(nrows*sizeof(int));
    ja = (int*)malloc(nnzMax*sizeof(int));
    a  = (double*)malloc(nnzMax*sizeof(double));
    /* Allocate the source and result vectors */
    x = (double*)malloc(nrows*sizeof(double));
    y = (double*)malloc(nrows*sizeof(double));

    /* Create a pentadiagonal matrix, representing very roughly a finite
       difference approximation to the Laplacian on a square n x n mesh */
    row = 0;
    nnz = 0;
    for (i=0; i<n; i++) {
	for (j=0; j<n; j++) {
	    ia[row] = nnz;
	    if (i>0) { ja[nnz] = row - n; a[nnz] = -1.0; nnz++; }
	    if (j>0) { ja[nnz] = row - 1; a[nnz] = -1.0; nnz++; }
	    ja[nnz] = row; a[nnz] = 4.0; nnz++;
	    if (j<n-1) { ja[nnz] = row + 1; a[nnz] = -1.0; nnz++; }
	    if (i<n-1) { ja[nnz] = row + n; a[nnz] = -1.0; nnz++; }
	    row++;
	}
    }
    ia[row] = nnz;
	
    
    //for (int cx=0; cx<nrows; cx++)
    //{
    //    printf("%d   ",ja[cx]);
//	if ((cx+1) % n == 0 && cx != 0) {printf("\n");}
//    }
    /*
    for (int cx=0; cx<nrows; cx++)
    {
        for(int cy=0; cy<nrows; cy++)
        {
             printf("%d     ", ja[cx][cy]);
        }
        printf("\n");
    }*/


    /* Create the source (x) vector */
    for (i=0; i<nrows; i++) x[i] = 1.0;

    /* Perform a matrix-vector multiply: y = A*x */
    /* Warning: To use this for timing, you need to (a) handle cold start
       (b) perform enough tests to make timing quantum relatively small */
    ts = mysecond();
    for (row=0; row<nrows; row++) {
	double sum = 0.0;
	for (idx=ia[row]; idx<ia[row+1]; idx++) {
	    sum += a[idx] * x[ja[idx]];
	}
	y[row] = sum;
    }
    t = mysecond() - ts;
    /* Compute with the result to keep the compiler for marking the
       code as dead */
    for (row=0; row<nrows; row++) {
	if (y[row] < 0) {
	    fprintf(stderr,"y[%d]=%f, fails consistency test\n", row, y[row]);
	}
    }
    printf("Time for Sparse Ax, nrows=%d, nnz=%d, T = %f\n", nrows, nnz, t);
    
    flops = nnz*2 / t;
    printf("Flops = %f\n", flops);

    bandwidth = (nnz + nrows) * (sizeof(int) + sizeof(double)) / t /1000000 ;  // make it Mb/s
    printf("bandwidth =  %f\n", bandwidth);

    free(ia); free(ja); free(a); free(x); free(y);
    return 0;
}
