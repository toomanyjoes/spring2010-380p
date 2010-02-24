				/*sri*/
/********************************************************************************
!                              INTEL CONFIDENTIAL
!   Copyright(C) 2006-2007 Intel Corporation. All Rights Reserved.
!   The source code contained  or  described herein and all documents related to
!   the source code ("Material") are owned by Intel Corporation or its suppliers
!   or licensors.  Title to the  Material remains with  Intel Corporation or its
!   suppliers and licensors. The Material contains trade secrets and proprietary
!   and  confidential  information of  Intel or its suppliers and licensors. The
!   Material  is  protected  by  worldwide  copyright  and trade secret laws and
!   treaty  provisions. No part of the Material may be used, copied, reproduced,
!   modified, published, uploaded, posted, transmitted, distributed or disclosed
!   in any way without Intel's prior express written permission.
!   No license  under any  patent, copyright, trade secret or other intellectual
!   property right is granted to or conferred upon you by disclosure or delivery
!   of the Materials,  either expressly, by implication, inducement, estoppel or
!   otherwise.  Any  license  under  such  intellectual property  rights must be
!   express and approved by Intel in writing.
!
!*******************************************************************************
!   Content : MKL Sparse BLAS C example
!
!*******************************************************************************
!
! Consider the matrix A (see Appendix 'Sparse Storage Formats for Sparse Blas 
! level 2-3')
!
!                 |   1       -1      0   -3     0   |
!                 |  -2        5      0    0     0   |
!   A    =        |   0        0      4    6     4   |,
!                 |  -4        0      2    7     0   |
!                 |   0        8      0    0    -5   |
!
!  
!  The matrix A is represented in the zero-based compressed sparse row storage 
!  scheme with the help of three arrays  (see Appendix 'Sparse Matrix Storage') 
!  as follows:
!
!         values = (1 -1 -3 -2 5 4 6 4 -4 2 7 8 -5) 
!         columns = (0 1 3 0 1 2 3 4 0 2 3 1 4) 
!         rowIndex = (0  3  5  8  11 13)
!
!  The representation of the matrix A  given above is the 2's variation. Two
!  integer arrays pointerB and pointerE instead of the array rowIndex are used in 
!  the NIST variation of variation of the compressed sparse row format. Thus the 
!  arrays values and columns are the same for the both variations. The arrays 
!  pointerB and pointerE for the matrix A are defined as follows:
!                          pointerB = (0 3  5  8 11)
!                          pointerE = (3 5  8 11 13) 
!  It's easy to see that 
!                    pointerB[i]= rowIndex[i] for i=0, ..4;
!                    pointerE[i]= rowIndex[i+1] for i=0, ..4.
!
!
!  The test performs the following operations : 
!    
!       2. The code computes (A)*S = F using MKL_DCSRMV where S is a vector
!
!	y := alpha*A*x + beta*y (alpha=1, beta=0).
!*******************************************************************************
*/
#include <stdio.h>
#include "mkl_types.h"
#include "mkl_spblas.h"

int main() {
//*******************************************************************************
//     Definition arrays for sparse representation of  the matrix A in 
//     the compressed sparse row format: 
//*******************************************************************************

	int M;
	int NNZ;
	char *filename = "sparse_matrix";
	double		*values;   //[NNZ]
	MKL_INT		*columns;  //[NNZ]
	MKL_INT		*rowIndex; //[M+1]
	read_matrices(filename, &values, &columns, &rowIndex, &M, &NNZ);

	MKL_INT		m = M, nnz = NNZ;
	MKL_INT		*pointerB, *pointerE; 
	pointerB = (MKL_INT *) malloc(sizeof(MKL_INT) * M);
	pointerE = (MKL_INT *) malloc(sizeof(MKL_INT) * M);

//*******************************************************************************
//    Declaration of local variables : 
//*******************************************************************************
	double		*input_vec = (double *)malloc(sizeof(double)*M);
	double		*output = (double *)malloc(sizeof(double)*M);
	double		alpha = 1.0, beta = 0.0;
	MKL_INT		i, j, is;
	char		transa;
	char		matdescra[6];

	for(i=0; i<M; ++i) {
		input_vec[i] = 1.0;
		output[i]    = 0.0;
	}

		printf("\n EXAMPLE PROGRAM FOR COMPRESSED SPARSE ROW FORMAT ROUTINES \n");

//*******************************************************************************
// Task 2.    Obtain matrix-vector multiply (U+I)' *sol --> rhs
//
//    Let us form the arrays pointerB and pointerE for the NIST's 
//	  variation of the compressed sparse row format using the array 
//    rowIndex. 
//
//*******************************************************************************
		for (i = 0; i < m; i++) {
			pointerB[i] = rowIndex[i];
			pointerE[i] = rowIndex[i+1];
		};

		transa = 'n';
		matdescra[0] = 'g';
		matdescra[3] = 'c';

		mkl_dcsrmv(&transa, &m, &m, &alpha, matdescra, values, columns, pointerB, pointerE, input_vec, &beta, output);

		printf("                             \n");
		printf("   OUTPUT DATA FOR MKL_DCSRMV\n");
		printf("   WITH TRIANGULAR MATRIX    \n");
		for (i = 0; i < m; i++) {
			printf("%7.1f\n", output[i]);
		};
		printf("-----------------------------------------------\n");
	return 0;
}


void read_matrices(char *filename, double **values, MKL_INT **columns, MKL_INT **rowIndex, MKL_INT *M, MKL_INT *NNZ)
{
        /*file format is not checked.. we can use the mmio.h methods at the end.*/
        FILE *fp = fopen(filename, "r");
        unsigned char read_char;
        char read_buf[1024];

        int i;

        double *A;
        MKL_INT *JA;
        MKL_INT *IA;

        MKL_INT m;
        MKL_INT n;
        MKL_INT num_non_zero_elements;

        while(!feof(fp)) {
                fscanf(fp, "%c",&read_char);
                /*printf("\n The line starts with %c",read_char);*/
                if(read_char == '%') {
                        /*printf("Entered\n");*/
                        fgets(read_buf, 1024, fp);
                        /*printf("\n The string read was %s",read_buf);*/
                        continue;
                } else {
                        fseek(fp, -1L, SEEK_CUR);
                        break;
                }
        }
        fscanf(fp, "%d %d %d\n",&m, &n, &num_non_zero_elements); //n is neglected, TA can assume square matricies.
	*M = m;
	*NNZ = num_non_zero_elements;

        /*printf("\nThe m=%d n=%d num=%d\n",m,n,num_non_zero_elements);*/

        A = (double *)malloc(sizeof(double)*num_non_zero_elements);
        JA = (int *)malloc(sizeof(int)*num_non_zero_elements);
        IA = (int *)malloc(sizeof(int)*(m+1));
	i=m+1;
	while(i) {
		fscanf(fp, "%d\n",&(IA[i]));
		--i;
	}
        while(!feof(fp)) {
                fscanf(fp, "%d %lf\n",&(JA[i]), &(A[i])); /*redundant writes happening*/
		++i;
                /*printf("\n%d %d", row_index, col_index);*/
                /*getchar();*/
        }
        fclose(fp);

        *values = A;
        *rowIndex = IA;
        *columns = JA;
        return;
}

