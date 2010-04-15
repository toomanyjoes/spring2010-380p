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
/*
// --- trans -------------------------------------------------------------------

int bli_is_notrans( char trans )
{
	return ( trans == BLIS_NO_TRANSPOSE ); 
}

int bli_is_trans( char trans )
{
	return ( trans == BLIS_TRANSPOSE ); 
}

int bli_is_conjnotrans( char trans )
{
	return ( trans == BLIS_CONJ_NO_TRANSPOSE ); 
}

int bli_is_conjtrans( char trans )
{
	return ( trans == BLIS_CONJ_TRANSPOSE ); 
}

// --- conj --------------------------------------------------------------------

int bli_is_noconj( char conj )
{
	return ( conj == BLIS_NO_CONJUGATE ); 
}

int bli_is_conj( char conj )
{
	return ( conj == BLIS_CONJUGATE ); 
}

// --- uplo --------------------------------------------------------------------

int bli_is_lower( char uplo )
{
	return ( uplo == BLIS_LOWER_TRIANGULAR ); 
}

int bli_is_upper( char uplo )
{
	return ( uplo == BLIS_UPPER_TRIANGULAR );
}

// --- side --------------------------------------------------------------------

int bli_is_left( char side )
{
	return ( side == BLIS_LEFT ); 
}

int bli_is_right( char side )
{
	return ( side == BLIS_RIGHT ); 
}

// --- diag --------------------------------------------------------------------

int bli_is_nonunit( char diag )
{
	return ( diag == BLIS_NONUNIT_DIAG ); 
}

int bli_is_unit( char diag )
{
	return ( diag == BLIS_UNIT_DIAG ); 
}

// --- storage-related ---------------------------------------------------------

int bli_is_col_storage( int rs, int cs )
{
	return ( rs == 1 ); 
}

int bli_is_row_storage( int rs, int cs )
{
	return ( cs == 1 ); 
}

int bli_is_gen_storage( int rs, int cs )
{
	return ( !bli_is_col_storage( rs, cs ) && 
             !bli_is_row_storage( rs, cs ) ); 
}

int bli_is_vector( int m, int n )
{
	return ( m == 1 || n == 1 ); 
}

// --- dimension-related -------------------------------------------------------

int bli_zero_dim1( int m )
{
	return ( m == 0 ); 
}
*/
def bli_zero_dim2( m: int, n: int ): bool
{
	return ( m == 0 || n == 0 ); 
}
/*
int bli_zero_dim3( int m, int k, int n )
{
	return ( m == 0 || k == 0 || n == 0 ); 
}
*/
