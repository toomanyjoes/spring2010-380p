/*
   libflame
   An object-based infrastructure for developing high-performance
   dense linear algebra libraries.

   Copyright (C) 2010, The University of Texas

   libflame is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.

   libflame is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with libflame; if you did not receive a copy, see
   http://www.gnu.org/licenses/.

   For more information, please contact us at flame@cs.utexas.edu or
   send mail to:

   Field G. Van Zee and/or
   Robert A. van de Geijn
   The University of Texas at Austin
   Department of Computer Sciences
   1 University Station C0500
   Austin TX 78712
*/

//#include "FLAME.h"
use constants, bli_is, dgemv;
/*
void bli_sgemv( char transa, char conjx, int m, int n, float* alpha, float* a, int a_rs, int a_cs, float* x, int incx, float* beta, float* y, int incy )
{
	int lda, inca;

	// Return early if possible.
	if ( bli_zero_dim2( m, n ) ) return;

	// Initialize with values assuming column-major storage.
	lda  = a_cs;
	inca = a_rs;

	// If A is a row-major matrix, then we can use the underlying column-major
	// BLAS implementation by fiddling with the parameters.
	if ( bli_is_row_storage( a_rs, a_cs ) )
	{
		bli_swap_ints( m, n );
		bli_swap_ints( lda, inca );
		bli_toggle_trans( transa );
	}

	bli_sgemv_blas( transa,
	                m,
	                n,
	                alpha,
	                a, lda,
	                x, incx,
	                beta,
	                y, incy );
}
*/
//def bli_dgemv( char transa, char conjx, int m, int n, double* alpha, double* a, int a_rs, int a_cs, double* x, int incx, double* beta, double* y, int incy )
def bli_dgemv( transa: string, conjx: string, m: int, n: int, alpha: real, a: [?aDom] real, a_rs: int,  a_cs: int, x: [?xDom] real, incx: int, beta: real, y: [? yDom] real, incy: int )
{
	var lda, inca: int;

	// Return early if possible.
	if ( bli_zero_dim2( m, n ) ) then return;

	// Initialize with values assuming column-major storage.
	//lda  = a_cs;
	//inca = a_rs;

	// If A is a row-major matrix, then we can use the underlying column-major
	// BLAS implementation by fiddling with the parameters.
	/*if ( bli_is_row_storage( a_rs, a_cs ) )
	{
		bli_swap_ints( m, n );
		bli_swap_ints( lda, inca );
		bli_toggle_trans( transa );
	}*/

	bli_dgemv_blas( transa,
	                m,
	                n,
	                alpha,
	                a, lda,
	                x, incx,
	                beta,
	                y, incy );
}
/*
void bli_cgemv( char transa, char conjx, int m, int n, scomplex* alpha, scomplex* a, int a_rs, int a_cs, scomplex* x, int incx, scomplex* beta, scomplex* y, int incy )
{
	scomplex* buff_zero = FLA_COMPLEX_PTR( FLA_ZERO );
	scomplex* buff_one  = FLA_COMPLEX_PTR( FLA_ONE );
	scomplex* x_conj;
	scomplex* ax;
	int       lda, inca;
	int       n_x;
	int       incx_conj;
	int       incax;

	// Return early if possible.
	if ( bli_zero_dim2( m, n ) ) return;

	// Initialize with values assuming column-major storage.
	lda  = a_cs;
	inca = a_rs;

	// If A is a row-major matrix, then we can use the underlying column-major
	// BLAS implementation by fiddling with the parameters.
	if ( bli_is_row_storage( a_rs, a_cs ) )
	{
		bli_swap_ints( m, n );
		bli_swap_ints( lda, inca );
		bli_toggle_trans( transa );
	}

	// Initialize with values assuming no conjugation of x.
	x_conj    = x;
	incx_conj = incx;

	// We need a temporary vector for the cases when x is conjugated, and
	// also for the cases where A is conjugated.
	if ( bli_is_conj( conjx ) || bli_is_conjnotrans( transa ) )
	{
		if ( bli_does_trans( transa ) ) n_x = m;
		else                            n_x = n;

		x_conj    = FLA_malloc( n_x * sizeof( *x ) );
		incx_conj = 1;

		bli_ccopyv( conjx,
		            n_x,
		            x,      incx,
                    x_conj, incx_conj );
	}

	// We want to handle the conjnotrans case, but without explicitly
	// conjugating A. To do so, we leverage the fact that computing the
	// product conj(A) * x is equivalent to computing conj( A * conj(x) ).
	if ( bli_is_conjnotrans( transa ) )
	{
		// We need a temporary vector for the product A * conj(x), which is
		// conformal to y. We know we are not transposing, so y is length m.
		ax    = FLA_malloc( m * sizeof( *x ) );
		incax = 1;
		
		// Start by conjugating the contents of the temporary copy of x.
		bli_cconjv( n,
		            x_conj, incx_conj );

		// Compute A * conj(x) where x is the temporary copy of x created above.
		bli_cgemv_blas( BLIS_NO_TRANSPOSE,
		                m,
		                n,
		                buff_one,
		                a,      lda,
		                x_conj, incx_conj,
		                buff_zero,
		                ax, incax );

		// Scale y by beta.
		bli_cscalv( BLIS_NO_CONJUGATE,
                    m,
                    beta,
                    y, incy );

		// And finally, accumulate alpha * conj( A * conj(x) ) into y.
		bli_caxpyv( BLIS_CONJUGATE,
                    m,
                    alpha,
                    ax, incax,
                    y,  incy);

		// Free the temporary vector for Ax.
		FLA_free( ax );
	}
	else // notrans, trans, or conjtrans
	{
		bli_cgemv_blas( transa,
		                m,
		                n,
		                alpha,
		                a,      lda,
		                x_conj, incx_conj,
		                beta,
		                y, incy );
	}

	if ( bli_is_conj( conjx ) || bli_is_conjnotrans( transa ) )
		FLA_free( x_conj );
}

void bli_zgemv( char transa, char conjx, int m, int n, dcomplex* alpha, dcomplex* a, int a_rs, int a_cs, dcomplex* x, int incx, dcomplex* beta, dcomplex* y, int incy )
{
	dcomplex* buff_zero = FLA_DOUBLE_COMPLEX_PTR( FLA_ZERO );
	dcomplex* buff_one  = FLA_DOUBLE_COMPLEX_PTR( FLA_ONE );
	dcomplex* x_conj;
	dcomplex* ax;
	int       lda, inca;
	int       n_x;
	int       incx_conj;
	int       incax;

	// Return early if possible.
	if ( bli_zero_dim2( m, n ) ) return;

	// Initialize with values assuming column-major storage.
	lda  = a_cs;
	inca = a_rs;

	// If A is a row-major matrix, then we can use the underlying column-major
	// BLAS implementation by fiddling with the parameters.
	if ( bli_is_row_storage( a_rs, a_cs ) )
	{
		bli_swap_ints( m, n );
		bli_swap_ints( lda, inca );
		bli_toggle_trans( transa );
	}

	// Initialize with values assuming no conjugation of x.
	x_conj    = x;
	incx_conj = incx;

	// We need a temporary vector for the cases when x is conjugated, and
	// also for the cases where A is conjugated.
	if ( bli_is_conj( conjx ) || bli_is_conjnotrans( transa ) )
	{
		if ( bli_does_trans( transa ) ) n_x = m;
		else                            n_x = n;

		x_conj    = FLA_malloc( n_x * sizeof( *x ) );
		incx_conj = 1;

		bli_zcopyv( conjx,
		            n_x,
		            x,      incx,
                    x_conj, incx_conj );
	}

	// We want to handle the conjnotrans case, but without explicitly
	// conjugating A. To do so, we leverage the fact that computing the
	// product conj(A) * x is equivalent to computing conj( A * conj(x) ).
	if ( bli_is_conjnotrans( transa ) )
	{
		// We need a temporary vector for the product A * conj(x), which is
		// conformal to y. We know we are not transposing, so y is length m.
		ax    = FLA_malloc( m * sizeof( *x ) );
		incax = 1;
		
		// Start by conjugating the contents of the temporary copy of x.
		bli_zconjv( n,
		            x_conj, incx_conj );

		// Compute A * conj(x) where x is the temporary copy of x created above.
		bli_zgemv_blas( BLIS_NO_TRANSPOSE,
		                m,
		                n,
		                buff_one,
		                a,      lda,
		                x_conj, incx_conj,
		                buff_zero,
		                ax,     incax );

		// Scale y by beta.
		bli_zscalv( BLIS_NO_CONJUGATE,
                    m,
                    beta,
                    y, incy );

		// And finally, accumulate alpha * conj( A * conj(x) ) into y.
		bli_zaxpyv( BLIS_CONJUGATE,
                    m,
                    alpha,
                    ax, incax,
                    y,  incy);

		// Free the temporary vector for Ax.
		FLA_free( ax );
	}
	else // notrans, trans, or conjtrans
	{
		bli_zgemv_blas( transa,
		                m,
		                n,
		                alpha,
		                a,      lda,
		                x_conj, incx_conj,
		                beta,
		                y,      incy );
	}

	if ( bli_is_conj( conjx ) || bli_is_conjnotrans( transa ) )
		FLA_free( x_conj );
}

// --- Classic routine wrappers ---

void bli_sgemv_blas( char transa, int m, int n, float* alpha, float* a, int lda, float* x, int incx, float* beta, float* y, int incy )
{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
	enum CBLAS_ORDER     cblas_order = CblasColMajor;
	enum CBLAS_TRANSPOSE cblas_transa;

	FLA_Param_map_blis_to_blas_trans( transa, &cblas_transa );

	cblas_sgemv( cblas_order,
	             cblas_transa,
	             m,
	             n,
	             *alpha,
	             a, lda,
	             x, incx,
	             *beta,
	             y, incy );
#else
	char blas_transa;

	FLA_Param_map_blis_to_blas_trans( transa, &blas_transa );

	FLA_C2F( sgemv )( &blas_transa,
	                  &m,
	                  &n,
	                  alpha,
	                  a, &lda,
	                  x, &incx,
	                  beta,
	                  y, &incy );
#endif
}
*/
//void bli_dgemv_blas( char transa, int m, int n, double* alpha, double* a, int lda, double* x, int incx, double* beta, double* y, int incy )
def bli_dgemv_blas( transa: string, m: int, n: int, alpha: real, a: [?aDom] real, lda: int, x: [?xDom] real, incx: int, beta: real, y: [?yDom] real, incy: int )
{
//#ifdef FLA_ENABLE_CBLAS_INTERFACES
	//enum CBLAS_ORDER    { cblas_order = CblasColMajor };
	//enum CBLAS_TRANSPOSE { cblas_transa };

	//FLA_Param_map_blis_to_blas_trans( transa, &cblas_transa );
	//FLA_Param_map_blis_to_blas_trans( transa, CBLAS_TRANSPOSE );

	/*cblas_dgemv( cblas_order,
	             cblas_transa,
	             m,
	             n,
	             alpha,
	             a, lda,
	             x, incx,
	             beta,
	             y, incy );*/
	blas_dgemv( //cblas_order,
	             //cblas_transa,
	             m,
	             n,
	             alpha,
	             a, lda,
	             x, incx,
	             beta,
	             y, incy );
/*#else
	char blas_transa;

	FLA_Param_map_blis_to_blas_trans( transa, &blas_transa );

	FLA_C2F( dgemv )( &blas_transa,
	                  &m,
	                  &n,
	                  alpha,
	                  a, &lda,
	                  x, &incx,
	                  beta,
	                  y, &incy );
#endif*/
}
/*
void bli_cgemv_blas( char transa, int m, int n, scomplex* alpha, scomplex* a, int lda, scomplex* x, int incx, scomplex* beta, scomplex* y, int incy )
{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
	enum CBLAS_ORDER     cblas_order = CblasColMajor;
	enum CBLAS_TRANSPOSE cblas_transa;

	FLA_Param_map_blis_to_blas_trans( transa, &cblas_transa );

	cblas_cgemv( cblas_order,
	             cblas_transa,
	             m,
	             n,
	             alpha,
	             a, lda,
	             x, incx,
	             beta,
	             y, incy );
#else
	char blas_transa;

	FLA_Param_map_blis_to_blas_trans( transa, &blas_transa );

	FLA_C2F( cgemv )( &blas_transa,
	                  &m,
	                  &n,
	                  alpha,
	                  a, &lda,
	                  x, &incx,
	                  beta,
	                  y, &incy );
#endif
}

void bli_zgemv_blas( char transa, int m, int n, dcomplex* alpha, dcomplex* a, int lda, dcomplex* x, int incx, dcomplex* beta, dcomplex* y, int incy )
{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
	enum CBLAS_ORDER     cblas_order = CblasColMajor;
	enum CBLAS_TRANSPOSE cblas_transa;

	FLA_Param_map_blis_to_blas_trans( transa, &cblas_transa );

	cblas_zgemv( cblas_order,
	             cblas_transa,
	             m,
	             n,
	             alpha,
	             a, lda,
	             x, incx,
	             beta,
	             y, incy );
#else
	char blas_transa;

	FLA_Param_map_blis_to_blas_trans( transa, &blas_transa );

	FLA_C2F( zgemv )( &blas_transa,
	                  &m,
	                  &n,
	                  alpha,
	                  a, &lda,
	                  x, &incx,
	                  beta,
	                  y, &incy );
#endif
}
**/
