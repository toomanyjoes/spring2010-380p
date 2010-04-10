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
use constants, FLA_Error_wrapper;

//#ifdef FLA_ENABLE_NON_CRITICAL_CODE

def FLA_Gemm_nn_unb_var1( alpha: FLA_Obj, A: FLA_Obj, B: FLA_Obj, beta: FLA_Obj, C:FLA_Obj ): FLA_Error
{
  var AT, A0, AB, a1t, A2: FLA_Obj;

  var CT, C0, CB, c1t, C2: FLA_Obj;

  FLA_Error_wrapper.FLA_Scal_external( beta, C );

  FLA_Part_2x1( A,    AT, 
                      AB,            0, FLA_TOP );

  FLA_Part_2x1( C,    CT, 
                      CB,            0, FLA_TOP );

  while FLA_Obj_length( AT ) < FLA_Obj_length( A ) {

    FLA_Repart_2x1_to_3x1( AT,                A0, 
                        /* ** */            /* *** */
                                              a1t, 
                           AB,                A2,        1, FLA_BOTTOM );

    FLA_Repart_2x1_to_3x1( CT,                C0, 
                        /* ** */            /* *** */
                                              c1t, 
                           CB,                C2,        1, FLA_BOTTOM );

    /*------------------------------------------------------------*/

    /* c1t  = a1t * B + c1t    */
    /* c1t' = B' * a1t' + c1t' */
    FLA_Gemv_external( FLA_TRANSPOSE, alpha, B, a1t, FLA_ONE, c1t );

    /*------------------------------------------------------------*/

    FLA_Cont_with_3x1_to_2x1( AT,                A0, 
                                                  a1t, 
                            /* ** */           /* *** */
                              AB,                A2,     FLA_TOP );

    FLA_Cont_with_3x1_to_2x1( CT,                C0, 
                                                  c1t, 
                            /* ** */           /* *** */
                              CB,                C2,     FLA_TOP );

  }

  return FLA_SUCCESS;
}

//#endif
