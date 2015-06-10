#ifndef SOLVERMATH_H_
#include "SolverMath.h"
#endif

template<class T, int NR, int NC>
inline void
am_v_prod(T* vC, const T *matA, const T *vB, const T alpha) {
	// Performs vC = alpha * matA * vB. Assumes row major ordering in mat.
	register int i, j, k = 0;
	for (i = 0; i < NR; i++) {
		vC[i] = (T) 0.0;
		for (j = 0; j < NC; j++) {
			vC[i] += matA[k++] * vB[j];
		}
		vC[i] *= alpha;
	}
}
 
template<class T, int NR, int NC>
inline void
minus_m_v_prod(T* vC, const T *matA, const T *vB) {
	// Performs vecC = -matA * vecB. Assumes row major ordering in mat.
	register int i, j, k = 0;
	for (i = 0; i < NR; i++) {
		vC[i] = (T) 0.0;
		for (j = 0; j < NC; j++) {
			vC[i] -= matA[k++] * vB[j];
		}
	}
}
 
template<class T, int NR, int NC>
inline void
m_v_prod(T* vC, const T *matA, const T *vB) {
	// Performs vC = alpha * matA * vB. Assumes row major ordering in mat.
	register int i, j, k = 0;
	for (i = 0; i < NR; i++) {
		vC[i] = 0.0;
		for (j = 0; j < NC; j++) {
			vC[i] += matA[k++] * vB[j];
		}
	}
}
 
template<class T, int NR, int NC>
inline void
add_am_v_prod(T* vC, const T *matA, const T *vB, const T alpha) {
	// Performs vC += alpha * matA * vB. Assumes row major ordering in mat.
	register int i, j, k = 0;
	T sum;
	for (i = 0; i < NR; i++) {
		sum = (T) 0.0;
		for (j = 0; j < NC; j++) {
			sum += matA[k++] * vB[j];
		}
		vC[i] += alpha * sum;
	}
}
 
template<class T, int NR, int NC>
inline void
add_m_v_prod(T* vC, const T *matA, const T *vB) {
	// Performs vC += matA * vB. Assumes row major ordering in mat.
	register int i, j, k = 0;
	for (i = 0; i < NR; i++) {
		for (j = 0; j < NC; j++) {
			vC[i] += matA[k++] * vB[j];
		}
	}
}

template<class T, int NR, int NC>
inline void
sub_am_v_prod(T* vC, const T *matA, const T *vB, const T alpha) {
	// Performs vC += alpha * matA * vB.
	// Assumes row major ordering in mat.
	register int i, j, k = 0;
	T sum;
	for (i = 0; i < NR; i++) {
		sum = (T) 0.0;
		for (j = 0; j < NC; j++) {
			sum += matA[k++] * vB[j];
		}
		vC[i] -= alpha * sum;
	}
}
 
template<class T, int NR, int NC>
inline void
sub_m_v_prod(T* vC, const T *matA, const T *vB) {
	// Performs vC -= alpha * matA * vB.
	// Assumes row major ordering in mat.
	register int i, j, k = 0;
	for (i = 0; i < NR; i++) {
		for (j = 0; j < NC; j++) {
			vC[i] -= matA[k++] * vB[j];
		}
	}
}

template<class T, int NR>
inline void
sub_a_v_prod(T* vC, const T *vB, const T alpha) {
	for (int i = 0; i < NR; i++) {
		vC[i] -= alpha * vB[i];
	}
}




