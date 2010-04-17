#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "FLAME.h"

int main(int argc, char *argv[])
{
  int m, n, nfirst, nlast, ninc, i, irep,
    nrepeats;

  double
    dtime,
    dtime_best,
    gflops,
    diff;

	struct timeval begin, end;


  FLA_Obj
    A, B, C;
  
  /* Initialize FLAME */
  FLA_Init( );

  /* Timing trials for matrix sizes n=nfirst to nlast in increments 
     of ninc will be performed */
  printf( "%% enter rows (m), and cols (n):" );
  scanf( "%d%d", &m, &n );
  printf( "%% %d %d\n", m, n );

    /* Allocate space for the matrix and vectors */

    FLA_Obj_create( FLA_DOUBLE, m, n, &A );
    FLA_Obj_create( FLA_DOUBLE, m, n, &B );
    FLA_Obj_create( FLA_DOUBLE, m, n, &C );
  
    /* Generate random matrices A and random vector x */
    FLA_Random_matrix( A );
    FLA_Random_matrix( B );
   
    FLA_Obj_show("A=[\n", A, "%lf", "\n];");
    FLA_Obj_show("B=[\n", B, "%lf", "\n];");
    
    
    gettimeofday(&begin, NULL);
    
    FLA_Gemm(FLA_NO_TRANSPOSE, FLA_NO_TRANSPOSE, FLA_ONE, A, B, FLA_ONE, C);
    
    gettimeofday(&end, NULL);
    
    FLA_Obj_show("C=[\n", C, "%lf", "\n];");
    
    double beginsec = (double)begin.tv_sec + ((double)begin.tv_usec/1000000.0);
	 double endsec = (double)end.tv_sec + ((double)end.tv_usec/1000000.0);
	 printf("Time: %lf\n",endsec-beginsec);
	 
    FLA_Obj_free(&A);
	 FLA_Obj_free(&B);
	 FLA_Obj_free(&C);

 }
