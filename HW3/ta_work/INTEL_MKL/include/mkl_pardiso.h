/*******************************************************************************
*                              INTEL CONFIDENTIAL
*   Copyright(C) 2004-2007 Intel Corporation. All Rights Reserved.
*   The source code contained  or  described herein and all documents related to
*   the source code ("Material") are owned by Intel Corporation or its suppliers
*   or licensors.  Title to the  Material remains with  Intel Corporation or its
*   suppliers and licensors. The Material contains trade secrets and proprietary
*   and  confidential  information of  Intel or its suppliers and licensors. The
*   Material  is  protected  by  worldwide  copyright  and trade secret laws and
*   treaty  provisions. No part of the Material may be used, copied, reproduced,
*   modified, published, uploaded, posted, transmitted, distributed or disclosed
*   in any way without Intel's prior express written permission.
*   No license  under any  patent, copyright, trade secret or other intellectual
*   property right is granted to or conferred upon you by disclosure or delivery
*   of the Materials,  either expressly, by implication, inducement, estoppel or
*   otherwise.  Any  license  under  such  intellectual property  rights must be
*   express and approved by Intel in writing.
*
********************************************************************************
*   Content : MKL PARDISO C header file
*
*           Contains interface to PARDISO.
*
********************************************************************************
*/
#if !defined( __MKL_PARDISO_H )

#define __MKL_PARDISO_H

#include "mkl_dss.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined( _WIN32 ) || defined( _WIN64 )
#define pardiso_ PARDISO
#else
#define PARDISO pardiso_
#endif

extern  void pardiso_(
	_MKL_DSS_HANDLE_t *,   _INTEGER_t *,     _INTEGER_t *,
	_INTEGER_t *,           _INTEGER_t *,     _INTEGER_t *,
	_DOUBLE_PRECISION_t *,  _INTEGER_t *,     _INTEGER_t *,
	_INTEGER_t *,           _INTEGER_t *,     _INTEGER_t *,
	_INTEGER_t *,           _DOUBLE_PRECISION_t *, _DOUBLE_PRECISION_t *,
   _INTEGER_t *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


