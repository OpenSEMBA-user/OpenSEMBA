/*
 * SolverMath.h
 *
 *  Created on: Sep 11, 2012
 *      Author: luis
 */
#ifndef SOLVERMATH_H_
#define SOLVERMATH_H_

// BLAS-L3 function for product. vecC = alpha * matA * vecB.
template<class T, int NR, int NC>
void
am_v_prod(T* vC, const T *matA, const T *vB, const T alpha);
// BLAS-L3 function for product. vecC = -matA * vecB.
template<class T, int NR, int NC>
void
minus_m_v_prod(T* vC, const T *matA, const T *vB);
// BLAS-L3 function for product. vecC = matA * vecB.
template<class T, int NR, int NC>
void
m_v_prod(T* vC, const T *matA, const T *vB);
// BLAS-L3 function for product. vecC += alpha * matA * vecB.
template<class T, int NR, int NC>
void
add_am_v_prod(T* vC, const T *matA, const T *vB, const T alpha);
// BLAS-L3 function for product. vecC += matA * vecB.
template<class T, int NR, int NC>
void
add_m_v_prod(T* vC, const T *matA, const T *vB);
// BLAS-L3 function for product. vecC -= matA * vecB.
template<class T, int NR, int NC>
void
sub_am_v_prod(T* vC, const T *matA, const T *vB, const T alpha);
// BLAS-L3 function for product. vecC -= matA * vecB.
template<class T, int NR, int NC>
void
sub_m_v_prod(T* vC, const T *matA, const T *vB);
// BLAS-L3 function for product. vecC -= alpha * vecB.
template<class T, int NR>
void
sub_a_v_prod(T* vC, const T *vB, const T alpha);

#include "SolverMath.hpp"

#endif

