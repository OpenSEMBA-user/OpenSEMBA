/*
 * Field.cpp
 *
 *  Created on: Sep 23, 2014
 *      Author: luis
 */


#ifndef FIELD_H_
#include "Field.h"
#endif

template<class T, int D>
Field<T,D>::Field() {
	val_ = NULL;
	size_ = 0;
}

template<class T, int D>
Field<T,D>::~Field() {
	if (val_ != NULL) {
		delete [] val_;
	}
}

template<class T, int D>
inline const T*
Field<T,D>::operator()(const uint i) const {
	assert(i < D);
	return &val_[size_*i];
}

template<class T, int D>
inline T*
Field<T,D>::set(const uint i) const {
	assert(i < D);
	return &val_[size_*i];
}

template<class T, int D>
inline T
Field<T,D>::operator[](const uint i) const {
	return val_[i];
}

template<class T, int D>
inline void
Field<T,D>::setSize(const uint siz) {
	size_ = siz;
	val_ = new T[D*siz];
}

template<class T, int D>
inline void
Field<T,D>::setToZero() {
	for (uint i = 0; i < size_*D; i++) {
		val_[i] = (T) 0.0;
	}
}

template<class T, int D>
inline void
Field<T,D>::set(const uint i, const CartesianVector<T,D>& vec) {
	for (uint j = 0; j < D; j++) {
		val_[j * size_ + i] = vec(j);
	}
}

template<class T, int D>
inline void
Field<T,D>::set(const uint i, const T& num) {
	for (uint j = 0; j < D; j++) {
		val_[j * size_ + i] = num;
	}
}

template<class T, int D>
inline void
Field<T,D>::setToOne() {
	for (uint i = 0; i < size_*D; i++) {
		val_[i] = (T) 1.0;
	}
}

template<class T, int D>
inline void
Field<T,D>::setToRandom(const double min, const double max) {
	double range = max - min;
	srand (1);
	for (uint i = 0; i < size_*D; i++) {
		val_[i] = range * rand() / (RAND_MAX + (double) 1) + min;
	}
}

template<class T, int D>
inline void
Field<T,D>::prod(
 const uint init,
 const uint end,
 const T param) {
	for (uint d = 0; d < D; d++) {
		for (uint i = init; i < end; i++) {
			val_[d*size_ + i] *= param;
		}
	}
}


template<class T, int D>
inline void
Field<T,D>::prod_omp(const uint init, const uint end, const T param) {
	uint i;
	for (uint d = 0; d < D; d++) {
#		pragma omp parallel for private(i)
		for (i = init; i < end; i++) {
			val_[d*size_ + i] *= param;
		}
	}
}

template<class T, int D>
inline void
Field<T,D>::copy(const uint init, const uint end,
		const Field<T, D>& field) {
	for (uint d = 0; d < D; d++) {
		for (uint i = init; i < end; i++) {
			val_[d*size_ + i] = field.val_[d*size_ + i];
		}
	}
}

template<class T, int D>
inline void
Field<T,D>::addProd(
 const uint init, const uint end,
 const Field<T, D>& field, const T param) {
	for (uint d = 0; d < D; d++) {
		for (uint i = init; i < end; i++) {
			val_[d*size_ + i] += field.val_[d*size_ + i] * param;
		}
	}
}

template<class T, int D>
inline void
Field<T,D>::addProd_omp(
 const uint init, const uint end,
 const Field<T, D>& field, const T param) {
	uint i;
	for (uint d = 0; d < D; d++) {
#		pragma omp parallel for private(i)
		for (i = init; i < end; i++) {
			val_[d*size_ + i] += field.val_[d*size_ + i] * param;
		}
	}
}

template<class T, int D>
inline uint
Field<T,D>::getDOFs() const {
	return (D*size_);
}

template<class T, int D>
inline void
Field<T,D>::swap(
 Field<T, D>& param, const uint first, const uint last) {
	for (uint i = 0; i < D; i++) {
		for (uint k = first; k < last; k++) {
			T aux = val_[i*size_ + k];
			val_[i*size_ + k] = param(i)[k];
			param(i)[k] = aux;
		}
	}
}
