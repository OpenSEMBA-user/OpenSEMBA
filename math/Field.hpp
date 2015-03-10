/*
 * Field.cpp
 *
 *  Created on: Sep 23, 2014
 *      Author: luis
 */


#ifndef FIELD_H_
#include "Field.h"
#endif

template<class T, Int D>
Field<T,D>::Field() {
	val_ = NULL;
	size_ = 0;
}

template<class T, Int D>
Field<T,D>::Field(UInt size) {
   size_ = size;
   val_ = new (T) (size_ * D) ;
}

template<class T, Int D>
Field<T,D>::~Field() {
	if (val_ != NULL) {
		delete [] val_;
	}
}

template<class T, Int D>
inline const T*
Field<T,D>::operator()(const UInt i) const {
	assert(i < D);
	return &val_[size_*i];
}

template<class T, Int D>
inline CartesianVector<T,D>
Field<T,D>::getCVec(const UInt i) const {
   CartesianVector<T,D> res;
   for (UInt d = 0; d < D; d++) {
      res(d) = (*this)(d)[i];
   }
   return res;
}

template<class T, Int D>
inline T*
Field<T,D>::set(const UInt i) const {
	assert(i < D);
	return &val_[size_*i];
}

template<class T, Int D>
inline T
Field<T,D>::operator[](const UInt i) const {
	return val_[i];
}

template<class T, Int D>
inline void
Field<T,D>::setSize(const UInt siz) {
	size_ = siz;
	val_ = new T[D*siz];
}

template<class T, Int D>
inline void
Field<T,D>::setToZero() {
	for (UInt i = 0; i < size_*D; i++) {
		val_[i] = (T) 0.0;
	}
}

template<class T, Int D>
inline void
Field<T,D>::set(const UInt i, const CartesianVector<T,D>& vec) {
	for (UInt j = 0; j < D; j++) {
		val_[j * size_ + i] = vec(j);
	}
}

template<class T, Int D>
inline void
Field<T,D>::set(const UInt i, const T& num) {
	for (UInt j = 0; j < D; j++) {
		val_[j * size_ + i] = num;
	}
}

template<class T, Int D>
inline void
Field<T,D>::setToOne() {
	for (UInt i = 0; i < size_*D; i++) {
		val_[i] = (T) 1.0;
	}
}

template<class T, Int D>
inline void
Field<T,D>::setToRandom(const Real min, const Real max) {
	Real range = max - min;
	srand (1);
	for (UInt i = 0; i < size_*D; i++) {
		val_[i] = range * rand() / (RAND_MAX + (Real) 1) + min;
	}
}

template<class T, Int D>
inline void
Field<T,D>::prod(
 const UInt init,
 const UInt end,
 const T param) {
	for (UInt d = 0; d < D; d++) {
		for (UInt i = init; i < end; i++) {
			val_[d*size_ + i] *= param;
		}
	}
}


template<class T, Int D>
inline void
Field<T,D>::prod_omp(const UInt init, const UInt end, const T param) {
	UInt i;
	for (UInt d = 0; d < D; d++) {
#		pragma omp parallel for private(i)
		for (i = init; i < end; i++) {
			val_[d*size_ + i] *= param;
		}
	}
}

template<class T, Int D>
inline void
Field<T,D>::copy(const UInt init, const UInt end,
		const Field<T, D>& field) {
	for (UInt d = 0; d < D; d++) {
		for (UInt i = init; i < end; i++) {
			val_[d*size_ + i] = field.val_[d*size_ + i];
		}
	}
}

template<class T, Int D>
inline void
Field<T,D>::addProd(
 const UInt init, const UInt end,
 const Field<T, D>& field, const T param) {
	for (UInt d = 0; d < D; d++) {
		for (UInt i = init; i < end; i++) {
			val_[d*size_ + i] += field.val_[d*size_ + i] * param;
		}
	}
}

template<class T, Int D>
inline void
Field<T,D>::addProd_omp(
 const UInt init, const UInt end,
 const Field<T, D>& field, const T param) {
	UInt i;
	for (UInt d = 0; d < D; d++) {
#		pragma omp parallel for private(i)
		for (i = init; i < end; i++) {
			val_[d*size_ + i] += field.val_[d*size_ + i] * param;
		}
	}
}

template<class T, Int D>
inline UInt
Field<T,D>::getDOFs() const {
	return (D*size_);
}

template<class T, Int D>
inline UInt Field<T,D>::size() const {
   return size_;
}

template<class T, Int D>
inline void
Field<T,D>::swap(
 Field<T, D>& param, const UInt first, const UInt last) {
	for (UInt i = 0; i < D; i++) {
		for (UInt k = first; k < last; k++) {
			T aux = val_[i*size_ + k];
			val_[i*size_ + k] = param(i)[k];
			param.set(i)[k] = aux;
		}
	}
}
