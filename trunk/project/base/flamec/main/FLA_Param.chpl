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
use constants, FLA_Check;

// --- FLAME to BLAS/LAPACK mappings -------------------------------------------
/*
void FLA_Param_map_flame_to_blas_trans( FLA_Trans trans, void* blas_trans )
{
	if ( trans == FLA_NO_TRANSPOSE )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_TRANSPOSE* ) blas_trans ) = CblasNoTrans;
#else
		*( ( char*                 ) blas_trans ) = 'N';
#endif
	}
	else if ( trans == FLA_TRANSPOSE )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_TRANSPOSE* ) blas_trans ) = CblasTrans;
#else
		*( ( char*                 ) blas_trans ) = 'T';
#endif
	}
	else if ( trans == FLA_CONJ_TRANSPOSE )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_TRANSPOSE* ) blas_trans ) = CblasConjTrans;
#else
		*( ( char*                 ) blas_trans ) = 'C';
#endif
	}
	else
	{
		FLA_Check_error_code( FLA_INVALID_TRANS );
	}
}

void FLA_Param_map_flame_to_blas_uplo( FLA_Uplo uplo, void* blas_uplo )
{
	if ( uplo == FLA_LOWER_TRIANGULAR )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_UPLO* ) blas_uplo ) = CblasLower;
#else
		*( ( char*            ) blas_uplo ) = 'L';
#endif
	}
	else if ( uplo == FLA_UPPER_TRIANGULAR )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_UPLO* ) blas_uplo ) = CblasUpper;
#else
		*( ( char*            ) blas_uplo ) = 'U';
#endif
	}
	else
	{
		FLA_Check_error_code( FLA_INVALID_UPLO );
	}
}

void FLA_Param_map_flame_to_blas_side( FLA_Side side, void* blas_side )
{
	if ( side == FLA_LEFT )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_SIDE* ) blas_side ) = CblasLeft;
#else
		*( ( char*            ) blas_side ) = 'L';
#endif
	}
	else if ( side == FLA_RIGHT )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_SIDE* ) blas_side ) = CblasRight;
#else
		*( ( char*            ) blas_side ) = 'R';
#endif
	}
	else
	{
		FLA_Check_error_code( FLA_INVALID_SIDE );
	}
}

void FLA_Param_map_flame_to_blas_diag( FLA_Diag diag, void* blas_diag )
{
	if ( diag == FLA_NONUNIT_DIAG )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_DIAG* ) blas_diag ) = CblasNonUnit;
#else
		*( ( char*            ) blas_diag ) = 'N';
#endif
	}
	else if ( diag == FLA_UNIT_DIAG )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_DIAG* ) blas_diag ) = CblasUnit;
#else
		*( ( char*            ) blas_diag ) = 'U';
#endif
	}
	else
	{
		FLA_Check_error_code( FLA_INVALID_DIAG );
	}
}

void FLA_Param_map_flame_to_lapack_direct( FLA_Direct direct, void* lapack_direct )
{
	if ( direct == FLA_FORWARD )
	{
		*( ( char* ) lapack_direct ) = 'F';
	}
	else if ( direct == FLA_BACKWARD )
	{
		*( ( char* ) lapack_direct ) = 'B';
	}
	else
	{
		FLA_Check_error_code( FLA_INVALID_DIRECT );
	}
}

void FLA_Param_map_flame_to_lapack_storev( FLA_Store storev, void* lapack_storev )
{
	if ( storev == FLA_COLUMNWISE )
	{
		*( ( char* ) lapack_storev ) = 'C';
	}
	else if ( storev == FLA_ROWWISE )
	{
		*( ( char* ) lapack_storev ) = 'R';
	}
	else
	{
		FLA_Check_error_code( FLA_INVALID_STOREV );
	}
}

// --- FLAME to BLIS mappings --------------------------------------------------

void FLA_Param_map_flame_to_blis_trans( FLA_Trans trans, char* blis_trans )
{
	if ( trans == FLA_NO_TRANSPOSE )
	{
		*( ( char* ) blis_trans ) = BLIS_NO_TRANSPOSE;
	}
	else if ( trans == FLA_TRANSPOSE )
	{
		*( ( char* ) blis_trans ) = BLIS_TRANSPOSE;
	}
	else if ( trans == FLA_CONJ_NO_TRANSPOSE )
	{
		*( ( char* ) blis_trans ) = BLIS_CONJ_NO_TRANSPOSE;
	}
	else if ( trans == FLA_CONJ_TRANSPOSE )
	{
		*( ( char* ) blis_trans ) = BLIS_CONJ_TRANSPOSE;
	}
	else
	{
		FLA_Check_error_code( FLA_INVALID_TRANS );
	}
}
*/
def FLA_Param_map_flame_to_blis_conj( conj: FLA_Conj, blis_conj: string ): string
{
	var returnval: string;
	if conj == FLA_NO_CONJUGATE
	{
		//blis_conj = BLIS_NO_CONJUGATE;
		returnval = BLIS_NO_CONJUGATE;
	}
	else if conj == FLA_CONJUGATE
	{
		//blis_conj = BLIS_CONJUGATE;
		returnval = BLIS_CONJUGATE;
	}
	else
	{
		FLA_Check_error_code( FLA_INVALID_CONJ );
	}
        return returnval;
}
/*
void FLA_Param_map_flame_to_blis_uplo( FLA_Uplo uplo, char* blis_uplo )
{
	if ( uplo == FLA_LOWER_TRIANGULAR )
	{
		*( ( char* ) blis_uplo ) = BLIS_LOWER_TRIANGULAR;
	}
	else if ( uplo == FLA_UPPER_TRIANGULAR )
	{
		*( ( char* ) blis_uplo ) = BLIS_UPPER_TRIANGULAR;
	}
	else
	{
		FLA_Check_error_code( FLA_INVALID_UPLO );
	}
}

void FLA_Param_map_flame_to_blis_side( FLA_Side side, char* blis_side )
{
	if ( side == FLA_LEFT )
	{
		*( ( char* ) blis_side ) = BLIS_LEFT;
	}
	else if ( side == FLA_RIGHT )
	{
		*( ( char* ) blis_side ) = BLIS_RIGHT;
	}
	else
	{
		FLA_Check_error_code( FLA_INVALID_SIDE );
	}
}

void FLA_Param_map_flame_to_blis_diag( FLA_Diag diag, char* blis_diag )
{
	if ( diag == FLA_NONUNIT_DIAG )
	{
		*( ( char* ) blis_diag ) = BLIS_NONUNIT_DIAG;
	}
	else if ( diag == FLA_UNIT_DIAG )
	{
		*( ( char* ) blis_diag ) = BLIS_UNIT_DIAG;
	}
	else
	{
		FLA_Check_error_code( FLA_INVALID_DIAG );
	}
}

// --- BLIS to BLAS/LAPACK mappings --------------------------------------------

void FLA_Param_map_blis_to_blas_trans( char blis_trans, void* blas_trans )
{
	if ( bli_is_notrans( blis_trans ) || bli_is_conjnotrans( blis_trans ) )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_TRANSPOSE* ) blas_trans ) = CblasNoTrans;
#else
		*( ( char*                 ) blas_trans ) = 'N';
#endif
	}
	else if ( bli_is_trans( blis_trans ) )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_TRANSPOSE* ) blas_trans ) = CblasTrans;
#else
		*( ( char*                 ) blas_trans ) = 'T';
#endif
	}
	else if ( bli_is_conjtrans( blis_trans ))
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_TRANSPOSE* ) blas_trans ) = CblasConjTrans;
#else
		*( ( char*                 ) blas_trans ) = 'C';
#endif
	}
	else
	{
		FLA_Check_error_code( FLA_INVALID_TRANS );
	}
}

void FLA_Param_map_blis_to_blas_uplo( char blis_uplo, void* blas_uplo )
{
	if ( bli_is_lower( blis_uplo ) )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_UPLO* ) blas_uplo ) = CblasLower;
#else
		*( ( char*            ) blas_uplo ) = 'L';
#endif
	}
	else if ( bli_is_upper( blis_uplo ) )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_UPLO* ) blas_uplo ) = CblasUpper;
#else
		*( ( char*            ) blas_uplo ) = 'U';
#endif
	}
	else
	{
		FLA_Check_error_code( FLA_INVALID_UPLO );
	}
}

void FLA_Param_map_blis_to_blas_side( char blis_side, void* blas_side )
{
	if ( bli_is_left( blis_side ) )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_SIDE* ) blas_side ) = CblasLeft;
#else
		*( ( char*            ) blas_side ) = 'L';
#endif
	}
	else if ( bli_is_right( blis_side ) )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_SIDE* ) blas_side ) = CblasRight;
#else
		*( ( char*            ) blas_side ) = 'R';
#endif
	}
	else
	{
		FLA_Check_error_code( FLA_INVALID_SIDE );
	}
}

void FLA_Param_map_blis_to_blas_diag( char blis_diag, void* blas_diag )
{
	if ( bli_is_nonunit( blis_diag ) )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_DIAG* ) blas_diag ) = CblasNonUnit;
#else
		*( ( char*            ) blas_diag ) = 'N';
#endif
	}
	else if ( bli_is_unit( blis_diag ) )
	{
#ifdef FLA_ENABLE_CBLAS_INTERFACES
		*( ( enum CBLAS_DIAG* ) blas_diag ) = CblasUnit;
#else
		*( ( char*            ) blas_diag ) = 'U';
#endif
	}
	else
	{
		FLA_Check_error_code( FLA_INVALID_DIAG );
	}
}

// --- BLAS/LAPACK to FLAME mappings -------------------------------------------

void FLA_Param_map_blaslapack_to_flame_trans( char* trans, FLA_Trans* flame_trans )
{
	if      ( *trans == 'n' || *trans == 'N' )
		*flame_trans = FLA_NO_TRANSPOSE;
	else if ( *trans == 't' || *trans == 'T' )
		*flame_trans = FLA_TRANSPOSE;
	else if ( *trans == 'c' || *trans == 'C' )
		*flame_trans = FLA_CONJ_TRANSPOSE;
	else
		FLA_Check_error_code( FLA_INVALID_TRANS );
}

void FLA_Param_map_blaslapack_to_flame_uplo( char* uplo, FLA_Uplo* flame_uplo )
{
	if      ( *uplo == 'l' || *uplo == 'L' )
		*flame_uplo = FLA_LOWER_TRIANGULAR;
	else if ( *uplo == 'u' || *uplo == 'U' )
		*flame_uplo = FLA_UPPER_TRIANGULAR;
	else
		FLA_Check_error_code( FLA_INVALID_UPLO );
}

void FLA_Param_map_blaslapack_to_flame_side( char* side, FLA_Side* flame_side )
{
	if      ( *side == 'l' || *side == 'L' )
		*flame_side = FLA_LEFT;
	else if ( *side == 'r' || *side == 'R' )
		*flame_side = FLA_RIGHT;
	else
		FLA_Check_error_code( FLA_INVALID_SIDE );
}

void FLA_Param_map_blaslapack_to_flame_diag( char* diag, FLA_Diag* flame_diag )
{
	if      ( *diag == 'n' || *diag == 'N' )
		*flame_diag = FLA_NONUNIT_DIAG;
	else if ( *diag == 'u' || *diag == 'U' )
		*flame_diag = FLA_UNIT_DIAG;
	else
		FLA_Check_error_code( FLA_INVALID_DIAG );
}

// --- BLIS/LAPACK to FLAME mappings -------------------------------------------

void FLA_Param_map_blislapack_to_flame_trans( char* trans, FLA_Trans* flame_trans )
{
	if      ( *trans == 'n' || *trans == 'N' )
		*flame_trans = FLA_NO_TRANSPOSE;
	else if ( *trans == 't' || *trans == 'T' )
		*flame_trans = FLA_TRANSPOSE;
	else if ( *trans == 'c' || *trans == 'C' )
		*flame_trans = FLA_CONJ_NO_TRANSPOSE;
	else if ( *trans == 'h' || *trans == 'H' )
		*flame_trans = FLA_CONJ_TRANSPOSE;
	else
		FLA_Check_error_code( FLA_INVALID_TRANS );
}

void FLA_Param_map_blislapack_to_flame_uplo( char* uplo, FLA_Uplo* flame_uplo )
{
	if      ( *uplo == 'l' || *uplo == 'L' )
		*flame_uplo = FLA_LOWER_TRIANGULAR;
	else if ( *uplo == 'u' || *uplo == 'U' )
		*flame_uplo = FLA_UPPER_TRIANGULAR;
	else
		FLA_Check_error_code( FLA_INVALID_UPLO );
}

void FLA_Param_map_blislapack_to_flame_side( char* side, FLA_Side* flame_side )
{
	if      ( *side == 'l' || *side == 'L' )
		*flame_side = FLA_LEFT;
	else if ( *side == 'r' || *side == 'R' )
		*flame_side = FLA_RIGHT;
	else
		FLA_Check_error_code( FLA_INVALID_SIDE );
}

void FLA_Param_map_blislapack_to_flame_diag( char* diag, FLA_Diag* flame_diag )
{
	if      ( *diag == 'n' || *diag == 'N' )
		*flame_diag = FLA_NONUNIT_DIAG;
	else if ( *diag == 'u' || *diag == 'U' )
		*flame_diag = FLA_UNIT_DIAG;
	else
		FLA_Check_error_code( FLA_INVALID_DIAG );
}
*/

