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

use constants;


class FLA_Base_obj
{
  // from FLA_Base_obj
  /* Basic object description fields */
  //type t;
  var datatype: FLA_Datatype;	// FLA_Datatype  datatype;
  var elemtype: FLA_Elemtype;	// FLA_Elemtype  elemtype;
  var m: dim_t;	// dim_t         m;
  var n: dim_t;	// dim_t         n;
  var rs: dim_t;	// dim_t         rs;
  var cs: dim_t;	// dim_t         cs;
  var m_inner: dim_t;	// dim_t         m_inner;
  var n_inner: dim_t;	// dim_t         n_inner;
  var id: dim_t;	// unsigned long id;
  var m_index: dim_t;	// dim_t         m_index;
  var n_index: dim_t;	// dim_t         n_index;
  var buffer: [1..1, 1..1] real;	//  this array is later resized       void*         buffer;
}

class FLA_Obj: FLA_Base_obj
{
  // from FLA_Obj
  /* Basic object view description fields */
  var offm: dim_t;	// dim_t         offm;
  var offn: dim_t;	// dim_t         offn;
  //var m: dim_t;		// dim_t         m;
  //var n: dim_t;		// dim_t         n;
  //FLA_Base_obj* base;

  // constructor
/*  def FLA_Obj(type typ, datatype: int = 0, elemtype: int = 0, m: uint = 0, n: uint, rs: uint = 0, cs: uint = 0, m_inner: uint = 0, n_inner: uint = 0, id: uint = 0, m_index: uint = 0, n_index: uint = 0, buffer: [1,1] real = 0, offm: uint = 0, offn: uint = 0)
  {
    t = typ;
/*    offm = 0;
    offn = 0;
    n_index = 0;
    m_index = 0;
    id = 0;
    n_inner = 0;
    m_inner = 0;
    cs = 0;
    rs = 0;
    n = 0;
    m = 0;
    elemtype = 0;
    datatype = 0;
  }*/
  def FLA_Obj()
  {
    //t = real;
    offm = 0;
    offn = 0;
    n_index = 0;
    m_index = 0;
    id = 0;
    n_inner = 0;
    m_inner = 0;
    cs = 0;
    rs = 0;
    n = 0;
    m = 0;
    elemtype = 0;
    datatype = 0;
    buffer = 0.0;
  }
}



def FLA_Obj_create(datatype: FLA_Datatype, m: dim_t, n: dim_t, rs: dim_t, cs: dim_t, obj: FLA_Obj ): FLA_Error
{
  FLA_Obj_create_ext( datatype, FLA_SCALAR, m, n, m, n, rs, cs, obj );

  return FLA_SUCCESS;
}


def FLA_Obj_create_ext( datatype: FLA_Datatype, elemtype: FLA_Elemtype, m: dim_t, n: dim_t, m_inner: dim_t, n_inner: dim_t, rs: dim_t, cs: dim_t, obj: FLA_Obj ): FLA_Error
{
  var buffer_size: size_t;

  // Check the strides, and modify them if needed.
  if rs == 0 && cs == 0
  {
    // Default values induce column-major storage, except when m == 1,
    // because we dont want both strides to be unit.
    if m == 1 && n > 1 
    {
      rs = n;
      cs = 1;
    }
    else
    {
      rs = 1;
      cs = m;
    }
  }
  else if rs == 1 && cs == 1 
  {
    // If both strides are unit, this is probably a "lazy" request for a
    // single vector. Since rs == cs == 1 is only valid for scalars, we
    // might have to adjust one of the strides to make them valid for use
    // with the BLAS.
    if m > 1 && n == 1 
    {
      // Set the column stride to indicate that this is a column vector stored
      // in column-major order. This is necessary because, in some cases, we
      // have to satisify the error checking in the underlying BLAS library,
      // which expects the leading dimension to be set to at least m, even if
      // it will never be used for indexing since it is a vector and thus only
      // has one column of data.
      cs = m;
    }
    else if m == 1 && n > 1
    {
      // Set the row stride to indicate that this is a row vector stored
      // in row-major order.
      rs = n;
    }

    // Nothing needs to be done for the scalar case where m == n == 1.
  }

//  if FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING
//  {
//    FLA_Obj_create_ext_check( datatype, elemtype, m, n, m_inner, n_inner, rs, cs, obj );
//  }

  // Populate the fields in the view object.
  obj.m = m;	//obj->m                = m;
  obj.n = n;	//obj->n                = n;
  obj.offm = 0;	//obj->offm             = 0;
  obj.offn = 0;	//obj->offn             = 0;

  // Allocate the base object field.
  //obj->base             = ( FLA_Base_obj * ) FLA_malloc( sizeof( FLA_Base_obj ) );

  // Populate the fields in the base object.
  obj.datatype = datatype;	//obj->base->datatype   = datatype;
  obj.elemtype = elemtype;	//obj->base->elemtype   = elemtype;
  //obj->base->m          = m;
  //obj->base->n          = n;
  obj.m_inner = m_inner;	//obj->base->m_inner    = m_inner;
  obj.n_inner = n_inner;	//obj->base->n_inner    = n_inner;
  obj.id = 1;			//obj->base->id         = ( unsigned long ) obj->base;
  obj.m_index = 0;	//obj->base->m_index    = 0;
  obj.n_index = 0;	//obj->base->n_index    = 0;

  // Determine the amount of space we need to allocate based on the values of
  // the row and column strides.
  if rs == 1
  {
    // For column-major storage, use cs for computing the size of the buffer
    // to allocate.

    // Align the leading dimension to some user-defined address multiple,
    // if requested at configure-time.
    //cs = FLA_align_ldim( cs, FLA_Obj_elem_size( obj ) );

    // Compute the size of the buffer needed for the object we're creating.
    //buffer_size = ( cs: size_t) *
    //              ( n: size_t ) *
    //              ( size_t ) FLA_Obj_elem_size( obj );
  }
  else // if ( cs == 1 )
  {
    // For row-major storage, use rs for computing the size of the buffer
    // to allocate.

    // Align the leading dimension to some user-defined address multiple,
    // if requested at configure-time.
    //rs = FLA_align_ldim( rs, FLA_Obj_elem_size( *obj ) );

    // Compute the size of the buffer needed for the object we're creating.
    //buffer_size = ( size_t ) m *
    //              ( size_t ) rs *
    //              ( size_t ) FLA_Obj_elem_size( *obj );
  }

  // Allocate the base object's element buffer.
  //obj->base->buffer = FLA_malloc( buffer_size );
  obj.buffer = [1..m, 1,..n];

  // Save the row and column strides used in the memory allocation.
  obj.rs = rs;	//obj->base->rs     = rs;
  obj.cs = cs;	//obj->base->cs     = cs;

//#ifdef FLA_ENABLE_SUPERMATRIX
//  // Initialize SuperMatrix fields.
//  obj->base->n_read_tasks   = 0;
//  obj->base->read_task_head = NULL;
//  obj->base->read_task_tail = NULL;
//  obj->base->write_task     = NULL;
//#endif

  return FLA_SUCCESS;
}



def FLA_align_ldim( ldim: dim_t, elem_size: dim_t ): dim_t
{
//#ifdef FLA_ENABLE_MEMORY_ALIGNMENT
//  #ifdef FLA_ENABLE_LDIM_ALIGNMENT
    // Increase ldim so that ( ldim * elem_size ) is a multiple of the desired
    // alignment.
//    ldim = ( ( ldim * elem_size + FLA_MEMORY_ALIGNMENT_BOUNDARY - 1 ) / 
//             FLA_MEMORY_ALIGNMENT_BOUNDARY ) *
//           FLA_MEMORY_ALIGNMENT_BOUNDARY /
//           elem_size;
//  #endif
//#endif

  return ldim;
}


/*
FLA_Error FLA_Obj_create_conf_to( FLA_Trans trans, FLA_Obj obj_cur, FLA_Obj *obj_new )
{
  FLA_Datatype datatype;
  FLA_Elemtype elemtype;
  dim_t        m, n;
  dim_t        rs, cs;

  if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
    FLA_Obj_create_conf_to_check( trans, obj_cur, obj_new );

  datatype = FLA_Obj_datatype( obj_cur );
  elemtype = FLA_Obj_elemtype( obj_cur );

  // Query the dimensions of the existing object.
  if ( trans == FLA_NO_TRANSPOSE || trans == FLA_CONJ_NO_TRANSPOSE )
  {
    m = FLA_Obj_length( obj_cur );
    n = FLA_Obj_width( obj_cur );
  }
  else // if ( trans == FLA_TRANSPOSE || trans == FLA_CONJ_TRANSPOSE )
  {
    m = FLA_Obj_width( obj_cur );
    n = FLA_Obj_length( obj_cur );
  }

  // Query the row and column strides of the existing object. We don't care
  // about the actual leading dimension of the existing object, only whether
  // it is in row- or column-major format.
  rs = FLA_Obj_row_stride( obj_cur );
  cs = FLA_Obj_col_stride( obj_cur );

  if ( ( rs == 1 && cs == 1 ) )
  {
    // Do nothing. This special case will be handled by FLA_Obj_create_ext().
    ;
  }
  else if ( rs == 1 )
  {
    // For column-major storage, use the m dimension as the column stride.
    // Row stride is already set to 1.
    cs = m;
  }
  else if ( cs == 1 )
  {
    // For row-major storage, use the n dimension as the row stride.
    // Column stride is already set to 1.
    rs = n;
  }

  FLA_Obj_create_ext( datatype, elemtype, m, n, m, n, rs, cs, obj_new );

  return FLA_SUCCESS;
}


FLA_Error FLA_Obj_create_copy_of( FLA_Trans trans, FLA_Obj obj_cur, FLA_Obj *obj_new )
{
  // Create a new object conformal to the current object.
  FLA_Obj_create_conf_to( trans, obj_cur, obj_new );

  // Copy the contents of the current object to the new object.
  FLA_Copyt_external( trans, obj_cur, *obj_new );

  return FLA_SUCCESS;
}


FLA_Error FLA_Obj_create_without_buffer( FLA_Datatype datatype, dim_t m, dim_t n, FLA_Obj *obj )
{
  if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
    FLA_Obj_create_without_buffer_check( datatype, m, n, obj );

  // Populate the fields in the view object.
  obj->m                = m;
  obj->n                = n;
  obj->offm             = 0;
  obj->offn             = 0;

  // Allocate the base object field.
  obj->base             = ( FLA_Base_obj * ) FLA_malloc( sizeof( FLA_Base_obj ) );

  // Populate the fields in the base object.
  obj->base->datatype   = datatype;
  obj->base->elemtype   = FLA_SCALAR;
  obj->base->m          = m;
  obj->base->n          = n;
  obj->base->m_inner    = m;
  obj->base->n_inner    = n;
  obj->base->id         = ( unsigned long ) obj->base;
  obj->base->m_index    = 0;
  obj->base->n_index    = 0;

  // Set the row and column strides to invalid values.
  obj->base->rs         = 0;
  obj->base->cs         = 0;

  // Initialize the base object's element buffer to NULL.
  obj->base->buffer     = NULL;

#ifdef FLA_ENABLE_SUPERMATRIX
  // Initialize SuperMatrix fields.
  obj->base->n_read_tasks   = 0;
  obj->base->read_task_head = NULL;
  obj->base->read_task_tail = NULL;
  obj->base->write_task     = NULL;
#endif

  return FLA_SUCCESS;
}

*/

def FLA_Obj_create_constant( const_real: real, obj: FLA_Obj ): FLA_Error
{
  //int*      temp_i;
  //float*    temp_s;
  //double*   temp_d;
  //scomplex* temp_c;
  //dcomplex* temp_z;

  if FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING then
    FLA_Obj_create_constant_check( const_real, obj );

  FLA_Obj_create( FLA_CONSTANT, 1, 1, 0, 0, obj );

  //temp_i       = FLA_INT_PTR( *obj );
  //temp_s       = FLA_FLOAT_PTR( *obj );
  //temp_d       = FLA_DOUBLE_PTR( *obj );
  //temp_c       = FLA_COMPLEX_PTR( *obj );
  //temp_z       = FLA_DOUBLE_COMPLEX_PTR( *obj );

  //*temp_i      = ( int   ) const_real;
  //*temp_s      = ( float ) const_real;
  //*temp_d      =           const_real;
  //temp_c->real = ( float ) const_real;
  //temp_c->imag = ( float ) 0.0;
  //temp_z->real =           const_real;
  //temp_z->imag =           0.0;

  obj.buffer[1,1] = const_real;
  return FLA_SUCCESS;
}


/*
FLA_Error FLA_Obj_create_complex_constant( double const_real, double const_imag, FLA_Obj *obj )
{
  int*      temp_i;
  float*    temp_s;
  double*   temp_d;
  scomplex* temp_c;
  dcomplex* temp_z;

  if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
    FLA_Obj_create_complex_constant_check( const_real, const_imag, obj );

  FLA_Obj_create( FLA_CONSTANT, 1, 1, 0, 0, obj );

  temp_i       = FLA_INT_PTR( *obj );
  temp_s       = FLA_FLOAT_PTR( *obj );
  temp_d       = FLA_DOUBLE_PTR( *obj );
  temp_c       = FLA_COMPLEX_PTR( *obj );
  temp_z       = FLA_DOUBLE_COMPLEX_PTR( *obj );

  *temp_i      = ( int   ) const_real;
  *temp_s      = ( float ) const_real;
  *temp_d      =           const_real;
  temp_c->real = ( float ) const_real;
  temp_c->imag = ( float ) const_imag;
  temp_z->real =           const_real;
  temp_z->imag =           const_imag;

  return FLA_SUCCESS;
}



FLA_Error FLA_Obj_attach_buffer( void *buffer, dim_t rs, dim_t cs, FLA_Obj *obj )
{
  if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
    FLA_Obj_attach_buffer_check( buffer, rs, cs, obj );

  obj->base->buffer = buffer;
  obj->base->rs     = rs;
  obj->base->cs     = cs;

  return FLA_SUCCESS;
}



FLA_Error FLA_Obj_free( FLA_Obj *obj )
{
  if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
    FLA_Obj_free_check( obj );

  FLA_free(            obj->base->buffer );
  FLA_free( ( void * ) obj->base );

  obj->offm = 0;
  obj->offn = 0;
  obj->m    = 0;
  obj->n    = 0;

  return FLA_SUCCESS;
}



FLA_Error FLA_Obj_free_without_buffer( FLA_Obj *obj )
{
  if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
    FLA_Obj_free_without_buffer_check( obj );

  FLA_free( ( void * ) obj->base );

  obj->offm = 0;
  obj->offn = 0;
  obj->m    = 0;
  obj->n    = 0;

  return FLA_SUCCESS;
}
*/
