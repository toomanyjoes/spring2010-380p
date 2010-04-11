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
use constants, FLA_Check, FLA_Obj_set_to_scalar_check_module;

def FLA_Obj_set_to_scalar( alpha: FLA_Obj, A: FLA_Obj ): FLA_Error
{
  /*var datatype: FLA_Datatype;
  var        m, n: dim_t;
  var        rs, cs: dim_t;
  var        i, j: dim_t;
*/
  if FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING then
    FLA_Obj_set_to_scalar_check( alpha, A );

/*  datatype = FLA_Obj_datatype( A );
  m        = FLA_Obj_length( A );
  n        = FLA_Obj_width( A );
  rs       = FLA_Obj_row_stride( A );
  cs       = FLA_Obj_col_stride( A );
*/
  forall (i,j) in A.base.buffer.domain do
    A.base.buffer(i,j) = alpha.base.buffer(1,1);
/*
  switch ( datatype ){

  case FLA_FLOAT:
  {
    float *buff_A     = ( float * ) FLA_FLOAT_PTR( A );
    float *buff_alpha = ( float * ) FLA_FLOAT_PTR( alpha );
 
    for ( j = 0; j < n; j++ )
      for ( i = 0; i < m; i++ )
        buff_A[ j*cs + i*rs ] = *buff_alpha;

    break;
  }

  case FLA_DOUBLE:
  {
    double *buff_A     = ( double * ) FLA_DOUBLE_PTR( A );
    double *buff_alpha = ( double * ) FLA_DOUBLE_PTR( alpha );
 
    for ( j = 0; j < n; j++ )
      for ( i = 0; i < m; i++ )
        buff_A[ j*cs + i*rs ] = *buff_alpha;

    break;
  }

  case FLA_COMPLEX:
  {
    scomplex *buff_A     = ( scomplex * ) FLA_COMPLEX_PTR( A );
    scomplex *buff_alpha = ( scomplex * ) FLA_COMPLEX_PTR( alpha );
 
    for ( j = 0; j < n; j++ )
      for ( i = 0; i < m; i++ )
        buff_A[ j*cs + i*rs ] = *buff_alpha;

    break;
  }

  case FLA_DOUBLE_COMPLEX:
  {
    dcomplex *buff_A     = ( dcomplex * ) FLA_DOUBLE_COMPLEX_PTR( A );
    dcomplex *buff_alpha = ( dcomplex * ) FLA_DOUBLE_COMPLEX_PTR( alpha );
 
    for ( j = 0; j < n; j++ )
      for ( i = 0; i < m; i++ )
        buff_A[ j*cs + i*rs ] = *buff_alpha;

    break;
  }

  }
*/
  return FLA_SUCCESS;
}

/*

void FLA_Obj_extract_real_scalar( FLA_Obj alpha, double* alpha_value )
{
  if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
    FLA_Obj_extract_real_scalar_check( alpha, alpha_value );

  if ( FLA_Obj_is_single_precision( alpha ) )
    *alpha_value = ( double ) *FLA_FLOAT_PTR( alpha );
  else
    *alpha_value = *FLA_DOUBLE_PTR( alpha );
}



void FLA_Obj_extract_complex_scalar( FLA_Obj alpha, dcomplex* alpha_value )
{
  if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
    FLA_Obj_extract_complex_scalar_check( alpha, alpha_value );

  if ( FLA_Obj_is_single_precision( alpha ) )
  {
    scomplex temp = *FLA_COMPLEX_PTR( alpha );
    alpha_value->real = ( double ) temp.real;
    alpha_value->imag = ( double ) temp.imag;
  }
  else
    *alpha_value = *FLA_DOUBLE_COMPLEX_PTR( alpha );
}



FLA_Error FLA_Obj_set_diagonal_to_scalar( FLA_Obj alpha, FLA_Obj A )
{
  FLA_Datatype datatype;
  dim_t        j, min_m_n;
  dim_t        rs, cs;

  if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
    FLA_Obj_set_diagonal_to_scalar_check( alpha, A );

  datatype = FLA_Obj_datatype( A );
  min_m_n  = FLA_Obj_min_dim( A );
  rs       = FLA_Obj_row_stride( A );
  cs       = FLA_Obj_col_stride( A );

  switch ( datatype ){

  case FLA_FLOAT:
  {
    float *buff_A     = ( float * ) FLA_FLOAT_PTR( A );
    float *buff_alpha = ( float * ) FLA_FLOAT_PTR( alpha );

    for ( j = 0; j < min_m_n; j++ )
      buff_A[ j*cs + j*rs ] = *buff_alpha;

    break;
  }

  case FLA_DOUBLE:
  {
    double *buff_A     = ( double * ) FLA_DOUBLE_PTR( A );
    double *buff_alpha = ( double * ) FLA_DOUBLE_PTR( alpha );

    for ( j = 0; j < min_m_n; j++ )
      buff_A[ j*cs + j*rs ] = *buff_alpha;

    break;
  }

  case FLA_COMPLEX:
  {
    scomplex *buff_A     = ( scomplex * ) FLA_COMPLEX_PTR( A );
    scomplex *buff_alpha = ( scomplex * ) FLA_COMPLEX_PTR( alpha );

    for ( j = 0; j < min_m_n; j++ )
    {
      buff_A[ j*cs + j*rs ].real = buff_alpha->real;
      buff_A[ j*cs + j*rs ].imag = buff_alpha->imag;
    }

    break;
  }

  case FLA_DOUBLE_COMPLEX:
  {
    dcomplex *buff_A     = ( dcomplex * ) FLA_DOUBLE_COMPLEX_PTR( A );
    dcomplex *buff_alpha = ( dcomplex * ) FLA_DOUBLE_COMPLEX_PTR( alpha );

    for ( j = 0; j < min_m_n; j++ )
    {
      buff_A[ j*cs + j*rs ].real = buff_alpha->real;
      buff_A[ j*cs + j*rs ].imag = buff_alpha->imag;
    }

    break;
  }

  }

  return FLA_SUCCESS;
}



FLA_Error FLA_Obj_set_to_identity( FLA_Obj A )
{
  if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
    FLA_Obj_set_to_identity_check( A );

  FLA_Obj_set_to_scalar( FLA_ZERO, A );

  FLA_Obj_set_diagonal_to_scalar( FLA_ONE, A );

  return FLA_SUCCESS;
}



FLA_Error FLA_Obj_add_to_diagonal( void* diag_value, FLA_Obj A )
{
  FLA_Datatype datatype;
  dim_t        j, min_m_n;
  dim_t        rs, cs;

  if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
    FLA_Obj_add_to_diagonal_check( diag_value, A );

  datatype = FLA_Obj_datatype( A );
  min_m_n  = FLA_Obj_min_dim( A );
  rs       = FLA_Obj_row_stride( A );
  cs       = FLA_Obj_col_stride( A );

  switch ( datatype ){

  case FLA_FLOAT:
  {
    float *buff_A    = ( float * ) FLA_FLOAT_PTR( A );
    float *value_ptr = ( float * ) diag_value;

    for ( j = 0; j < min_m_n; j++ )
      buff_A[ j*cs + j*rs ] += *value_ptr;

    break;
  }

  case FLA_DOUBLE:
  {
    double *buff_A    = ( double * ) FLA_DOUBLE_PTR( A );
    double *value_ptr = ( double * ) diag_value;

    for ( j = 0; j < min_m_n; j++ )
      buff_A[ j*cs + j*rs ] += *value_ptr;

    break;
  }

  case FLA_COMPLEX:
  {
    scomplex *buff_A    = ( scomplex * ) FLA_COMPLEX_PTR( A );
    scomplex *value_ptr = ( scomplex * ) diag_value;

    for ( j = 0; j < min_m_n; j++ )
    {
      buff_A[ j*cs + j*rs ].real += value_ptr->real;
      buff_A[ j*cs + j*rs ].imag += value_ptr->imag;
    }

    break;
  }

  case FLA_DOUBLE_COMPLEX:
  {
    dcomplex *buff_A    = ( dcomplex * ) FLA_DOUBLE_COMPLEX_PTR( A );
    dcomplex *value_ptr = ( dcomplex * ) diag_value;

    for ( j = 0; j < min_m_n; j++ )
    {
      buff_A[ j*cs + j*rs ].real += value_ptr->real;
      buff_A[ j*cs + j*rs ].imag += value_ptr->imag;
    }

    break;
  }

  }

  return FLA_SUCCESS;
}



FLA_Error FLA_Obj_shift_diagonal( FLA_Obj sigma, FLA_Obj A )
{
  FLA_Datatype datatype_A;
  FLA_Datatype datatype_sigma;
  dim_t        j, min_m_n;
  dim_t        rs, cs;

  if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
    FLA_Obj_shift_diagonal_check( sigma, A );

  datatype_A     = FLA_Obj_datatype( A );
  datatype_sigma = FLA_Obj_datatype( sigma );
  min_m_n        = FLA_Obj_min_dim( A );
  rs             = FLA_Obj_row_stride( A );
  cs             = FLA_Obj_col_stride( A );
  
  switch( datatype_A ){

  case FLA_FLOAT:
  {
    float *buff_A     = ( float * ) FLA_FLOAT_PTR( A );
    float *buff_sigma = ( float * ) FLA_FLOAT_PTR( sigma );

    for ( j = 0; j < min_m_n; j++ )
      *( buff_A + j*cs + j*rs ) += *buff_sigma;

    break;
  }

  case FLA_DOUBLE:
  {
    double *buff_A     = ( double * ) FLA_DOUBLE_PTR( A );
    double *buff_sigma = ( double * ) FLA_DOUBLE_PTR( sigma );

    for ( j = 0; j < min_m_n; j++ )
      *( buff_A + j*cs + j*rs ) += *buff_sigma;

    break;
  }

  case FLA_COMPLEX:
  {
    if ( datatype_sigma == FLA_COMPLEX )
    {
      scomplex *buff_A     = ( scomplex * ) FLA_COMPLEX_PTR( A );
      scomplex *buff_sigma = ( scomplex * ) FLA_COMPLEX_PTR( sigma );
  
      for ( j = 0; j < min_m_n; j++ )
      {
        ( buff_A + j*cs + j*rs )->real += buff_sigma->real;
        ( buff_A + j*cs + j*rs )->imag += buff_sigma->imag;
      }
    }
    else
    {
      scomplex *buff_A     = ( scomplex * ) FLA_COMPLEX_PTR( A );
      float    *buff_sigma = ( float    * ) FLA_FLOAT_PTR( sigma );
  
      for ( j = 0; j < min_m_n; j++ )
      {
        ( buff_A + j*cs + j*rs )->real += *buff_sigma;
      }
    }

    break;
  }

  case FLA_DOUBLE_COMPLEX:
  {
    if ( datatype_sigma == FLA_DOUBLE_COMPLEX )
    {
      dcomplex *buff_A     = ( dcomplex * ) FLA_DOUBLE_COMPLEX_PTR( A );
      dcomplex *buff_sigma = ( dcomplex * ) FLA_DOUBLE_COMPLEX_PTR( sigma );

      for ( j = 0; j < min_m_n; j++ )
      {
        ( buff_A + j*cs + j*rs )->real += buff_sigma->real;
        ( buff_A + j*cs + j*rs )->imag += buff_sigma->imag;
      }
    }
    else
    {
      dcomplex *buff_A     = ( dcomplex * ) FLA_DOUBLE_COMPLEX_PTR( A );
      double   *buff_sigma = ( double   * ) FLA_DOUBLE_PTR( sigma );

      for ( j = 0; j < min_m_n; j++ )
      {
        ( buff_A + j*cs + j*rs )->real += *buff_sigma;
      }
    }

    break;
  }

  }

  return FLA_SUCCESS;
}



FLA_Error FLA_Obj_scale_diagonal( FLA_Obj alpha, FLA_Obj A )
{
  FLA_Datatype datatype_A;
  FLA_Datatype datatype_alpha;
  dim_t        j, min_m_n;
  dim_t        rs, cs;

  if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
    FLA_Obj_scale_diagonal_check( alpha, A );

  datatype_A     = FLA_Obj_datatype( A );
  datatype_alpha = FLA_Obj_datatype( alpha );
  min_m_n        = FLA_Obj_min_dim( A );
  rs             = FLA_Obj_row_stride( A );
  cs             = FLA_Obj_col_stride( A );
  
  switch( datatype_A ){

  case FLA_FLOAT:
  {
    float *buff_A     = ( float * ) FLA_FLOAT_PTR( A );
    float *buff_alpha = ( float * ) FLA_FLOAT_PTR( alpha );

    for ( j = 0; j < min_m_n; j++ )
      *( buff_A + j*cs + j*rs ) *= *buff_alpha;

    break;
  }

  case FLA_DOUBLE:
  {
    double *buff_A     = ( double * ) FLA_DOUBLE_PTR( A );
    double *buff_alpha = ( double * ) FLA_DOUBLE_PTR( alpha );

    for ( j = 0; j < min_m_n; j++ )
      *( buff_A + j*cs + j*rs ) *= *buff_alpha;

    break;
  }

  case FLA_COMPLEX:
  {
    if ( datatype_alpha == FLA_COMPLEX )
    {
      scomplex *buff_A     = ( scomplex * ) FLA_COMPLEX_PTR( A );
      scomplex *buff_alpha = ( scomplex * ) FLA_COMPLEX_PTR( alpha );
  
      for ( j = 0; j < min_m_n; j++ )
      {
        ( buff_A + j*cs + j*rs )->real *= buff_alpha->real;
        ( buff_A + j*cs + j*rs )->imag *= buff_alpha->imag;
      }
    }
    else
    {
      scomplex *buff_A     = ( scomplex * ) FLA_COMPLEX_PTR( A );
      float    *buff_alpha = ( float    * ) FLA_FLOAT_PTR( alpha );
  
      for ( j = 0; j < min_m_n; j++ )
      {
        ( buff_A + j*cs + j*rs )->real *= *buff_alpha;
      }
    }

    break;
  }

  case FLA_DOUBLE_COMPLEX:
  {
    if ( datatype_alpha == FLA_DOUBLE_COMPLEX )
    {
      dcomplex *buff_A     = ( dcomplex * ) FLA_DOUBLE_COMPLEX_PTR( A );
      dcomplex *buff_alpha = ( dcomplex * ) FLA_DOUBLE_COMPLEX_PTR( alpha );

      for ( j = 0; j < min_m_n; j++ )
      {
        ( buff_A + j*cs + j*rs )->real *= buff_alpha->real;
        ( buff_A + j*cs + j*rs )->imag *= buff_alpha->imag;
      }
    }
    else
    {
      dcomplex *buff_A     = ( dcomplex * ) FLA_DOUBLE_COMPLEX_PTR( A );
      double   *buff_alpha = ( double   * ) FLA_DOUBLE_PTR( alpha );

      for ( j = 0; j < min_m_n; j++ )
      {
        ( buff_A + j*cs + j*rs )->real *= *buff_alpha;
      }
    }

    break;
  }

  }

  return FLA_SUCCESS;
}



FLA_Error FLA_Obj_fshow( FILE* file, char *s1, FLA_Obj A, char *format, char *s2 )
{
  FLA_Datatype datatype;
  dim_t        i, j, m, n;
  dim_t        rs, cs;

  datatype = FLA_Obj_datatype( A );
  m        = FLA_Obj_length( A );
  n        = FLA_Obj_width( A );
  rs       = FLA_Obj_row_stride( A );
  cs       = FLA_Obj_col_stride( A );

  fprintf( file, "%s\n", s1 );

  switch ( datatype ){

  case FLA_FLOAT:
  {
    float *buffer = ( float * ) FLA_FLOAT_PTR( A );

    for ( i = 0; i < m; i++ )
    {
      for ( j = 0; j < n; j++ )
      {
        fprintf( file, format, buffer[ j*cs + i*rs ] );
        fprintf( file, " " );
      }
      fprintf( file, "\n" );
    }
    fprintf( file, "%s\n", s2 );
    break;
  }

  case FLA_DOUBLE:
  {
    double *buffer = ( double * ) FLA_DOUBLE_PTR( A );

    for ( i = 0; i < m; i++ )
    {
      for ( j = 0; j < n; j++ )
      {
        fprintf( file, format, buffer[ j*cs + i*rs ] );
        fprintf( file, " " );
      }
      fprintf( file, "\n" );
    }
    fprintf( file, "%s\n", s2 );
    break;
  }

  case FLA_COMPLEX:
  {
    scomplex *buffer = ( scomplex * ) FLA_COMPLEX_PTR( A );

    for ( i = 0; i < m; i++ )
    {
      for ( j = 0; j < n; j++ )
      {
        fprintf( file, format, buffer[ j*cs + i*rs ].real, buffer[ j*cs + i*rs ].imag );
        fprintf( file, " " );
      }
      fprintf( file, "\n" );
    }
    fprintf( file, "%s\n", s2 );
    break;
  }

  case FLA_DOUBLE_COMPLEX:
  {
    dcomplex *buffer = ( dcomplex * ) FLA_DOUBLE_COMPLEX_PTR( A );

    for ( i = 0; i < m; i++ )
    {
      for ( j = 0; j < n; j++ )
      {
        fprintf( file, format, buffer[ j*cs + i*rs ].real, buffer[ j*cs + i*rs ].imag );
        fprintf( file, " " );
      }
      fprintf( file, "\n" );
    }
    fprintf( file, "%s\n", s2 );
    break;
  }

  case FLA_INT:
  {
    int *buffer = ( int * ) FLA_INT_PTR( A );

    for ( i = 0; i < m; i++ )
    {
      for ( j = 0; j < n; j++ )
      {
        fprintf( file, format, buffer[ j*cs + i*rs ] );
        fprintf( file, " " );
      }
      fprintf( file, "\n" );
    }
    fprintf( file, "%s\n", s2 );
    break;
  }

  }

  return FLA_SUCCESS;
}



FLA_Error FLA_Obj_show( char *s1, FLA_Obj A, char *format, char *s2 )
{
  return FLA_Obj_fshow( stdout, s1, A, format, s2 );
}
*/
