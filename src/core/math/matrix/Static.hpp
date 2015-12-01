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

#include "Static.h"

namespace SEMBA {
namespace Math {
namespace Matrix {

template <class T, Size ROWS, Size COLS>
Static<T,ROWS,COLS>::Static() {
    for (Size i = 0; i < ROWS*COLS; i++) {
        _val[i] = T(0);
    }
}

template <class T, Size ROWS, Size COLS>
Static<T,ROWS,COLS>::Static(
        const Static<T,ROWS,COLS>& param) {
    Size nRnC = ROWS * COLS;
    for (Size i = 0; i < nRnC; i++) {
        _val[i] = (T) param._val[i];
    }
}

template <class T, Size ROWS, Size COLS>
Static<T,ROWS,COLS>::~Static() {
}

template <class T, Size ROWS, Size COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::operator=(
        const Static<T,ROWS,COLS>& param) {
    if (this == &param)
        return *this;
    for (Size i = 0; i < ROWS*COLS; i++)
        _val[i] = param._val[i];
    return *this;
}

template <class T, Size ROWS, Size COLS>
inline T Static<T,ROWS,COLS>::val(const Size i) const {
    return _val[i];
}

template <class T, Size ROWS, Size COLS>
inline T& Static<T,ROWS,COLS>::val(const Size i) {
    return _val[i];
}

template <class T, Size ROWS, Size COLS>
inline T Static<T,ROWS,COLS>::val(const Size row, const Size col) const {
    return _val[row * COLS + col];
}

template <class T, Size ROWS, Size COLS>
inline T& Static<T,ROWS,COLS>::val(const Size row, const Size col) {
    return _val[row * COLS + col];
}

template <class T, Size ROWS, Size COLS>
inline const T* Static<T,ROWS,COLS>::val() const {
    return _val;
}

template<class T, Size ROWS, Size COLS>
inline T Static<T,ROWS,COLS>::operator()(
        const Size row,
        const Size col) const {
    return _val[row * COLS + col];
}

template<class T, Size ROWS, Size COLS>
inline T& Static<T,ROWS,COLS>::operator()(const Size row, const Size col) {
    return _val[row * COLS + col];
}

template <class T, Size ROWS, Size COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::assign(
        const Static<Int,ROWS,COLS>& param) {
    for (Size i = 0; i < ROWS; i++) {
        for (Size j = 0; j < COLS; j++) {
            val(i,j) = param(i,j);
        }
    }
    return *this;
}

template <class T, Size ROWS, Size COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::setInDiagonal(
        const Vector::Cartesian<T,ROWS>& rhs) {
    static_assert(ROWS == COLS, "Requires squire matrix");
    for (Size i = 0; i < ROWS; i++) {
        val(i,i) = rhs(i);
    }
    return *this;
}

template <class T, Size ROWS, Size COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::operator=(
        const Dynamic<Real>& param) {
    assert(ROWS == param.nRows() && COLS == param.nCols());
    for (Size i = 0; i < ROWS; i++)
        for (Size j = 0; j < COLS; j++)
            val(i,j) = param(i,j);
    return *this;
}

template<class T, Size ROWS, Size COLS>
inline Static<T, ROWS, COLS> Static<T,ROWS,COLS>::operator+(
        Static<T, ROWS, COLS>& param) const {
    Static<T,ROWS,COLS> res;
    Size n = ROWS*COLS;
    for (Size i = 0; i < n; i++) {
        res._val[i] = _val[i] + param._val[i];
    }
    return res;
}

template <class T, Size ROWS, Size COLS>
void Static<T,ROWS,COLS>::printInfo() const {
    printInfo(ROWS, COLS);
}

template<class T, Size ROWS, Size COLS>
std::array<std::complex<Real>, ROWS>
        Static<T,ROWS,COLS>::getEigenvalues() const {
}

template <class T, Size ROWS, Size COLS>
void Static<T,ROWS,COLS>::printInfo(Size rows, Size cols) const {
    Size i, j;
    if (rows > ROWS || cols > COLS) {
        throw typename Error::Size();
    }
    std::cout << "Matrix type: Double";
    std::cout << "Dimensions: " << ROWS << "x" << COLS << std::endl;
    std::cout << "Stored values: " << std::endl;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++)
            std::cout << val(i,j) << " ";
        std::cout << std::endl;
    }
}

template<class T, Size ROWS, Size COLS>
Static<T,COLS,ROWS>&
Static<T,ROWS,COLS>::invert() {
    this->internal_();
    return *this;
}

template<class T, Size ROWS, Size COLS>
inline Size Static<T,ROWS,COLS>::nCols() const {
    return COLS;
}

template<class T, Size ROWS, Size COLS>
inline Size Static<T,ROWS,COLS>::nRows() const {
    return ROWS;
}

template<class T, Size ROWS, Size COLS>
Static<T,COLS,ROWS> Static<T,ROWS,COLS>::transpose() {
    Static<T,COLS,ROWS> res;
    for (Size i = 0; i < ROWS; i++) {
        for (Size j = 0; j < COLS; j++) {
            res(j,i) = val(i,j);
        }
    }
    return res;
}

template<class T, Size ROWS, Size COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::operator+=(
        const Real param) {
    for (Size i = 0; i < ROWS*COLS; i++)
        _val[i] += param;
    return *this;
}

template<class T, Size ROWS, Size COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::operator+=(
        const Static<T,ROWS,COLS>& param) {
    for (Size i = 0; i < ROWS*COLS; i++) {
        _val[i] += param._val[i];
    }
    return *this;
}

template<class T, Size ROWS, Size COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::operator-=(
        const Static<T,ROWS,COLS>& param) {
    for (Size i = 0; i < ROWS*COLS; i++) {
        _val[i] -= param._val[i];
    }
    return *this;
}

template<class T, Size ROWS, Size COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::operator*=(const T param) {
    for (Size i = 0; i < ROWS*COLS; i++) {
        _val[i] *= param;
    }
    return *this;
}

template<class T, Size ROWS, Size COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::operator/=(const T param) {
    for (Size i = 0; i < ROWS*COLS; i++) {
        _val[i] /= param;
    }
    return *this;
}

template<class T, Size ROWS, Size COLS>
Static<T,ROWS,COLS> Static<T,ROWS,COLS>::operator*(T param) const {
    Static<T,ROWS,COLS> res;
    for (Size i = 0; i < ROWS*COLS; i++) {
        res._val[i] = _val[i] * param;
    }
    return res;
}

template<class T, Size ROWS, Size COLS>
Vector::Cartesian<T,COLS> Static<T,ROWS,COLS>::operator*(
        Vector::Cartesian<T,COLS> rhs) const {
    Vector::Cartesian<T,COLS> res;
    for (Size i = 0; i < ROWS; i++) {
        for (Size j = 0; j < COLS; j++) {
            res(i) += this->val(i,j)*rhs(j);
        }
    }
    return res;
}

template<class T, Size ROWS, Size COLS>
bool
Static<T,ROWS,COLS>::operator==(
        const Static<T,ROWS,COLS>& param) const {
    for (Size i = 0; i < ROWS*COLS; i++) {
        Real diff = abs(_val[i] -param._val[i]);
        if (diff > std::numeric_limits<Real>::epsilon() * 1e2) {
            return false;
        }
    }
    return true;
}

template<class T, Size ROWS, Size COLS>
bool
Static<T,ROWS,COLS>::operator<(
        const Static<T,ROWS,COLS>& param) const {
    for (Size i = 0; i < (ROWS*COLS); i++) {
        if (val(i) < param.val(i)) {
            return true;
        }
        if (val(i) > param.val(i)) {
            return false;
        }
    }
    return false;
}

template<class T, Size ROWS, Size COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::operator=(
        const Dynamic<Int>& rhs) {
    assert(ROWS == rhs.nRows() && COLS == rhs.nCols());
    for (Size i = 0; i < ROWS; i++)
        for (Size j = 0; j < COLS; j++)
            val(i,j) = (T) rhs(i,j);
    return *this;
}

template<class T, Size ROWS, Size COLS>
void
Static<T,ROWS,COLS>::copy(std::vector<std::vector<T> > values) {
    assert(ROWS == values.size());
    for (Size i = 0; i < ROWS; i++) {
        assert(values[i].size() == COLS);
        for (Size j = 0; j < COLS; j++) {
            val(i,j) = values[i][j];
        }
    }
}


template<class T, class S, Size ROWS, Size COLS, Size NCOLSB>
Static<T,ROWS,NCOLSB> operator*(
        const Static<T,ROWS,COLS>& lhs,
        const Static<S,COLS,NCOLSB>& rhs) {
    Size i, j, k;
    Static<T,ROWS,NCOLSB> res;
    for (i = 0; i < ROWS; i++) {
        for (k = 0; k < COLS; k++) {
            for (j = 0; j < NCOLSB; j++) {
                res(i,j) += lhs(i,k) * rhs(k,j);
            }
        }
    }
    return res;
}

template<class T, Size ROWS, Size COLS>
Dynamic<T> operator*(
        const Static<T,ROWS,COLS>& lhs,
        const Dynamic<T>& rhs) {
    assert(COLS == rhs.nRows());
    Size i, j, k;
    Dynamic<T> res(ROWS, rhs.nCols());
    for (i = 0; i < ROWS; i++) {
        for (k = 0; k < COLS; k++) {
            for (j = 0; j < rhs.nCols(); j++) {
                res(i,j) += lhs(i,k) * rhs(k,j);
            }
        }
    }
    return res;
}

} /* namespace Matrix */
} /* namespace Math */
} /* namespace SEMBA */
