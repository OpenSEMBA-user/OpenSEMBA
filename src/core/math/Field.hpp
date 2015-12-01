// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#include "Field.h"

namespace SEMBA {
namespace Math {

template<class T, Size D>
Field<T,D>::Field() {
    val_ = NULL;
    size_ = 0;
}

template<class T, Size D>
Field<T,D>::Field(Size size) {
    size_ = size;
    val_ = new (T) (size_ * D) ;
}

template<class T, Size D>
Field<T,D>::~Field() {
    if (val_ != NULL) {
        delete [] val_;
    }
}

template<class T, Size D>
inline T* Field<T,D>::operator()(const Size i) {
    assert(i < D);
    return &val_[size_*i];
}

template<class T, Size D>
inline const T* Field<T,D>::operator()(const Size i) const {
    assert(i < D);
    return &val_[size_*i];
}

template<class T, Size D>
inline Vector::Cartesian<T,D> Field<T,D>::getCVec(const Size i) const {
    Vector::Cartesian<T,D> res;
    for (Size d = 0; d < D; d++) {
        res(d) = (*this)(d)[i];
    }
    return res;
}

template<class T, Size D>
inline T* Field<T,D>::set(const Size i) const {
    assert(i < D);
    return &val_[size_*i];
}

template<class T, Size D>
inline T Field<T,D>::operator[](const Size i) const {
    return val_[i];
}

template<class T, Size D>
inline void Field<T,D>::setSize(const Size siz) {
    size_ = siz;
    val_ = new T[D*siz];
}

template<class T, Size D>
inline void Field<T,D>::set(const Size i, const Vector::Cartesian<T,D>& vec) {
    for (Size j = 0; j < D; j++) {
        val_[j * size_ + i] = vec(j);
    }
}

template<class T, Size D>
inline void Field<T,D>::set(const Size i, const T& num) {
    for (Size j = 0; j < D; j++) {
        val_[j * size_ + i] = num;
    }
}

template<class T, Size D>
inline void Field<T,D>::setAll(const T& num) {
    for (Size i = 0; i < size_*D; i++) {
        val_[i] = (T) num;
    }
}

template<class T, Size D>
inline void Field<T,D>::setToRandom(const Real min, const Real max) {
    Real range = max - min;
    srand (1);
    for (Size i = 0; i < size_*D; i++) {
        val_[i] = range * rand() / (RAND_MAX + (Real) 1) + min;
    }
}

template<class T, Size D>
inline void Field<T,D>::prod(const Size init, const Size end,
                             const T param) {
    for (Size d = 0; d < D; d++) {
        for (Size i = init; i < end; i++) {
            val_[d*size_ + i] *= param;
        }
    }
}


template<class T, Size D>
inline void Field<T,D>::prod_omp(const Size init, const Size end,
                                 const T param) {
    Size i;
    for (Size d = 0; d < D; d++) {
#pragma omp parallel for private(i)
        for (i = init; i < end; i++) {
            val_[d*size_ + i] *= param;
        }
    }
}

template<class T, Size D>
inline void Field<T,D>::copy(const Size init, const Size end,
                             const Field<T, D>& field) {
    for (Size d = 0; d < D; d++) {
        for (Size i = init; i < end; i++) {
            val_[d*size_ + i] = field.val_[d*size_ + i];
        }
    }
}

template<class T, Size D>
inline void Field<T,D>::addProd(const Size init, const Size end,
                                const Field<T, D>& field, const T param) {
    for (Size d = 0; d < D; d++) {
        for (Size i = init; i < end; i++) {
            val_[d*size_ + i] += field.val_[d*size_ + i] * param;
        }
    }
}

template<class T, Size D>
inline void Field<T,D>::addProd_omp(const Size init, const Size end,
                                    const Field<T, D>& field, const T param) {
    Size i;
    for (Size d = 0; d < D; d++) {
#pragma omp parallel for private(i)
        for (i = init; i < end; i++) {
            val_[d*size_ + i] += field.val_[d*size_ + i] * param;
        }
    }
}

template<class T, Size D>
inline Size
Field<T,D>::getDOFs() const {
    return (D*size_);
}

template<class T, Size D>
inline Size Field<T,D>::size() const {
    return size_;
}

template<class T, Size D>
inline void Field<T,D>::swap(Field<T, D>& param,
                             const Size first,
                             const Size last) {
    for (Size i = 0; i < D; i++) {
        for (Size k = first; k < last; k++) {
            T aux = val_[i*size_ + k];
            val_[i*size_ + k] = param(i)[k];
            param.set(i)[k] = aux;
        }
    }
}

} /* namespace Math */
} /* namespace SEMBA */
