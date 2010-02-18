#ifndef _csr_trisolve_h
#define _csr_trisolve_h
/**
 * @file csr_trisolve.h
 * @author Mark Hoemmen
 * @since 18 July 2006
 * @date Time-stamp: <2008-07-16 10:53:58 mhoemmen>
 *
 * Reference implementation of CSR triangular solve.  Go to OSKI
 * (http://bebop.cs.berkeley.edu/oski/) for a better (faster)
 * implementation.
 *
 * Copyright (c) 2008, Regents of the University of California 
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the
 * following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright 
 *   notice, this list of conditions and the following disclaimer in 
 *   the documentation and/or other materials provided with the 
 *   distribution.
 *
 * * Neither the name of the University of California, Berkeley, nor
 *   the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior
 *   written permission.  
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <bebop/util/complex.h>

/**
 * x := L \ x;
 */
void
csr_lower_trisolve_double_real (double* x, const double* val, const int* ind, const int* ptr, const int n);

/**
 * x := U \ x;
 */
void
csr_upper_trisolve_double_real (double* x, const double* val, const int* ind, const int* ptr, const int n);

void
csr_lower_trisolve_double_complex (double_Complex* x, const double_Complex* val, 
				   const int* ind, const int* ptr, const int n);

void
csr_upper_trisolve_double_complex (double_Complex* x, const double_Complex* val, 
				   const int* ind, const int* ptr, const int n);

#endif /* _csr_trisolve_h */
