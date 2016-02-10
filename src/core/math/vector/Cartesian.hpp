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

#include "Cartesian.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <type_traits>

namespace SEMBA {
namespace Math {
namespace Vector {

template <class T, std::size_t D>
Cartesian<T,D>::Cartesian() {
    for (std::size_t i = 0; i < D; i++) {
        val[i] = T(0);
    }
}

template <class T, std::size_t D>
Cartesian<T,D>::Cartesian(const T val_) {
    for (std::size_t i = 0; i < D; i++) {
        val[i] = val_;
    }
}

template<class T, std::size_t D>
Cartesian<T,D>::Cartesian(T val_[D]) {
    for (std::size_t i = 0; i < D; i++) {
        val[i] = val_[i];
    }
}

template<class T, std::size_t D>
Cartesian<T,D>::Cartesian(const T val_[D]) {
    for (std::size_t i = 0; i < D; i++) {
        val[i] = val_[i];
    }
}


template <class T, std::size_t D>
Cartesian<T,D>::Cartesian(const T x, const T y, const T z) {
    assert(D==3);
    val[0] = x;
    val[1] = y;
    val[2] = z;
}

template <class T, std::size_t D>
Cartesian<T,D>::Cartesian(const Cartesian<T,D>& begin,
                          const Cartesian<T,D>& end) {
    for (std::size_t i = 0; i < D; i++) {
        val[i] = end.val[i]-begin.val[i];
    }
}

template <class T, std::size_t D> template<class U>
Cartesian<T,D>::Cartesian(const Cartesian<U,D>& param) {
    for (std::size_t i = 0; i < D; i++) {
        val[i] = (T) param.val[i];
    }
}

template<class T, std::size_t D>
Cartesian<T,D>::~Cartesian() {

}

template <class T, std::size_t D>
Cartesian<T,D>& Cartesian<T,D>::operator=(const T param) {
    for (std::size_t i = 0; i < D; i++) {
        val[i] = param;
    }
    return *this;
}

template <class T, std::size_t D> template<class U>
Cartesian<T,D>& Cartesian<T,D>::operator=(const Cartesian<U,D>& param) {
    for (std::size_t i = 0; i < D; i++) {
        val[i] = (T) param.val[i];
    }
    return *this;
}
//
//template <class T, std::size_t D>
//Cartesian<T,D>& Cartesian<T,D>::operator=(
//        const Cartesian<std::size_t,D>& param) {
//    for (std::size_t i = 0; i < D; i++) {
//        val[i] = (T) param.val[i];
//    }
//    return *this;
//}

template <class T, std::size_t D>
Cartesian<T,D>& Cartesian<T,D>::operator+=(const T param) {
    for (std::size_t i = 0; i < D; i++) {
        val[i] += param;
    }
    return *this;
}

template <class T, std::size_t D>
inline Cartesian<T,D>& Cartesian<T,D>::operator+=(
        const Cartesian<T,D>& param) {
    for (std::size_t i = 0; i < D; i++) {
        val[i] += param.val[i];
    }
    return *this;
}

template <class T, std::size_t D>
Cartesian<T,D>& Cartesian<T,D>::operator-=(const T param) {
    for (std::size_t i = 0; i < D; i++) {
        val[i] -= param;
    }
    return *this;
}

template <class T, std::size_t D>
Cartesian<T,D>& Cartesian<T,D>::operator-=(const Cartesian<T,D>& param) {
    for (std::size_t i = 0; i < D; i++) {
        val[i] -= param.val[i];
    }
    return *this;
}

template <class T, std::size_t D>
inline Cartesian<T,D>& Cartesian<T,D>::operator*=(const T param) {
    for (std::size_t i = 0; i < D; i++) {
        val[i] *= param;
    }
    return *this;
}

template <class T, std::size_t D>
inline Cartesian<T,D>& Cartesian<T,D>::operator/=(const T param) {
    for (std::size_t i = 0; i < D; i++) {
        val[i] /= param;
    }
    return *this;
}

template <class T, std::size_t D>
Cartesian<T,D> Cartesian<T,D>::operator+(const T param) const {
    Cartesian<T,D> res;
    for (std::size_t i = 0; i < D; i++) {
        res.val[i] = val[i] + param;
    }
    return res;
}

template <class T, std::size_t D>
Cartesian<T,D> Cartesian<T,D>::operator+(const Cartesian<T,D>& param) const {
    Cartesian<T,D> res;
    for (std::size_t i = 0; i < D; i++) {
        res.val[i] = val[i] + param.val[i];
    }
    return res;
}

template <class T, std::size_t D>
Cartesian<T,D>& Cartesian<T,D>::operator-() {
    for (std::size_t i = 0; i < D; i++) {
        val[i] = - val[i];
    }
    return *this;
}

template <class T, std::size_t D>
Cartesian<T,D> Cartesian<T,D>::operator-(const Cartesian<T,D>& param) const {
    Cartesian<T,D> res;
    for (std::size_t i = 0; i < D; i++) {
        res.val[i] = val[i] - param.val[i];
    }
    return res;
}

template <class T, std::size_t D>
Cartesian<T,D> Cartesian<T,D>::operator-(const T param) const {
    Cartesian<T,D> res;
    for (std::size_t i = 0; i<D; i++) {
        res.val[i] = val[i] - param;
    }
    return res;
}

template <class T, std::size_t D>
Cartesian<T,D> operator-(const T& lhs, const Cartesian<T,D>& rhs) {
    Cartesian<Real,D> res;
    for (std::size_t i = 0; i < D; i++) {
        res.val[i] = lhs - rhs.val[i];
    }
    return res;
}

template <class T, std::size_t D>
Cartesian<T,D> Cartesian<T,D>::operator*(const T param) const {
    Cartesian<T, D>  res;
    for (std::size_t i = 0; i < D; i++) {
        res.val[i] = val[i] * param;
    }
    return res;
}

template <class T, std::size_t D> inline
Cartesian<T,D> Cartesian<T,D>::operator*(const Cartesian<T,D>& param) const {
    Cartesian<T, D>  res;
    for (std::size_t i = 0; i < D; i++) {
        res.val[i] = val[i] * param.val[i];
    }
    return res;
}

template <class T, std::size_t D>
Cartesian<T,D> Cartesian<T,D>::operator/(const T param) const {
    Cartesian<T, D>  res;
    for (std::size_t i = 0; i < D; i++) {
        res.val[i] = val[i] / param;
    }
    return res;
}

template <class T, std::size_t D>
Cartesian<T,D> Cartesian<T,D>::operator^(const Cartesian<T,D>& param) const {
    // PURPOSE: Computes vectorial product.
    assert(D == 3);
    Cartesian<T,D> res;
    res.val[0] = val[1] * param.val[2] - param.val[1] * val[2];
    res.val[1] = val[2] * param.val[0] - param.val[2] * val[0];
    res.val[2] = val[0] * param.val[1] - param.val[0] * val[1];
    return res;
}

template <class T, std::size_t D>
inline T Cartesian<T,D>::dot(const Cartesian<T,D>& param) const {
    T res = T();
    for (std::size_t i = 0; i < D; i++) {
        res += val[i] * param.val[i];
    }
    return res;
}

template <class T, std::size_t D>
inline T Cartesian<T,D>::getMax() const {
    T res = val[0];
    for (std::size_t i = 1; i < D; i++) {
        if (val[i] > res) {
            res = val[i];
        }
    }
    return res;
}

template <class T, std::size_t D>
Cartesian<T,D>& Cartesian<T,D>::setAsBinary(UInt number) {
    assert(number < pow(2,D));
    for (std::size_t d = 0; d < D; d++) {
        val[D-d-1] = number % 2;
        number /= 2;
    }
    return *this;
}

template <class T, std::size_t D>
Cartesian<T,D>& Cartesian<T,D>::setWithMinimalComponents(
        const Cartesian<T,D>& rhs) {
    for(std::size_t d = 0; d < D; d++){
        if(val[d] > rhs.val[d]){
            val[d] = rhs.val[d];
        }
    }
    return *this;
}

template <class T, std::size_t D>
bool Cartesian<T,D>::operator==(const Cartesian<T, D>& param) const {
    return Util::equal((*this-param).norm(), 0.0, (*this+param).norm());
}

template <class T, std::size_t D>
inline bool Cartesian<T,D>::operator!=(const Cartesian<T,D>& param) const {
    return !(*this == param);
}

template <class T, std::size_t D>
bool Cartesian<T,D>::isContainedInPlane() const {
    return (this->isContainedInPlane(Constants::xy) ||
            this->isContainedInPlane(Constants::yz) ||
            this->isContainedInPlane(Constants::zx));
}

template <class T, std::size_t D>
bool Cartesian<T,D>::isContainedInPlane(
        const Constants::CartesianPlane plane) const {
    assert(D == 3);
    if (std::is_same<T, std::complex<Real>>::value) {
        switch (plane) {
        case Constants::xy:
            if (Util::equal(std::abs(val[2]), 0.0)) {
                return true;
            }
            break;
        case Constants::yz:
            if (Util::equal(std::abs(val[0]), 0.0)) {
                return true;
            }
            break;
        case Constants::zx:
            if (Util::equal(std::abs(val[1]), 0.0)) {
                return true;
            }
            break;
        }
        return false;
    } else {
        switch (plane) {
        case Constants::xy:
            if (Util::equal(std::abs(val[2]), 0.0)) {
                return true;
            }
            break;
        case Constants::yz:
            if (Util::equal(std::abs(val[0]), 0.0)) {
                return true;
            }
            break;
        case Constants::zx:
            if (Util::equal(std::abs(val[1]), 0.0)) {
                return true;
            }
            break;
        }
        return false;
    }
}

template<class T, std::size_t D>
inline bool Cartesian<T,D>::isCoplanar(const Cartesian<T, D>& param) const {
    return (*this - param).isContainedInPlane();
}

template <class T, std::size_t D>
inline T& Cartesian<T,D>::operator() (std::size_t pos) {
    assert(pos >= 0 && pos < D);
    return val[pos];
}

template <class T, std::size_t D>
inline T Cartesian<T,D>::operator() (std::size_t pos) const {
    assert(pos >= 0 && pos < D);
    return val[pos];
}

template <class T, std::size_t D>
inline T& Cartesian<T,D>::operator[] (std::size_t pos) {
    assert(pos >= 0 && pos < D);
    return val[pos];
}

template <class T, std::size_t D>
inline T Cartesian<T,D>::operator[] (std::size_t pos) const {
    assert(pos >= 0 && pos < D);
    return val[pos];
}

template <class T, std::size_t D>
inline Real Cartesian<T,D>::norm() const {
    Real sum = 0;
    for (std::size_t i = 0; i < D; i++) {
        sum += (Real) std::abs(val[i]) * std::abs(val[i]);
    }
    return sqrt(sum);
}


template <class T, std::size_t D>
Cartesian<T,D>& Cartesian<T,D>::cyclicPermutation(const std::size_t n) {
    Cartesian<T,D> valAux(0.0);
    for (std::size_t i = 0; i < D; i++) {
        valAux.val[(i+n)%D] = val[i];
    }
    *this=valAux;

    return *this;
}

template <class T, std::size_t D> inline
Cartesian<T,D>& Cartesian<T,D>::abs() {
    for (std::size_t i = 0; i < D; i++) {
        val[i] = (T) std::fabs(val[i]);
    }
    return *this;
}

template <class T, std::size_t D>
Cartesian<T,D>& Cartesian<T,D>::normalize() {
    Real nor = norm();
    for (std::size_t i = 0; i < D; i++) {
        val[i] /= (T)nor;
    }
    return *this;
}

template <class T, std::size_t D> inline
Cartesian<T,D>& Cartesian<T,D>::setPlusInfty() {
    for (std::size_t i = 0; i < D; i++) {
        if (std::numeric_limits<T>::has_infinity) {
            val[i] = std::numeric_limits<T>::infinity();
        } else {
            val[i] = std::numeric_limits<T>::max();
        }
    }
    return *this;
}

template <class T, std::size_t D> inline
Cartesian<T,D>& Cartesian<T,D>::setMinusInfty() {
    for (std::size_t i = 0; i < D; i++) {
        if (std::numeric_limits<T>::has_infinity) {
            val[i] = -std::numeric_limits<T>::infinity();
        } else {
            val[i] = std::numeric_limits<T>::min();
        }
    }
    return *this;
}

template <class T, std::size_t D>
std::string Cartesian<T,D>::toStr() const {
    std::stringstream ss;
    ss << "(";
    for (std::size_t i = 0; i < D; i++) {
        ss << val[i];
        if (i < D-1) {
            ss << " , ";
        }
    }
    ss << ")";
    return ss.str();
}

template <class T, std::size_t D>
void Cartesian<T,D>::printInfo() const {
    std::cout << toStr() << std::flush;
}

template<std::size_t D>
Cartesian<Real,D> operator+(const Cartesian<Int ,D> & lhs,
                            const Cartesian<Real,D>& rhs) {
    Cartesian<Real,D> res;
    for (std::size_t i = 0; i < D; i++) {
        res(i) = lhs(i) + rhs(i);
    }
    return res;
}

template<std::size_t D>
Cartesian<Real,D> operator/(const Cartesian<Int,D>& lhs,
                            const Real rhs) {
    Cartesian<Real,D> res;
    for (std::size_t i = 0; i < D; i++) {
        res(i) = (Real) lhs(i) / rhs;
    }
    return  res;
}

template<class T, std::size_t D>
bool operator< (const Cartesian<T,D>& lhs,
                const Cartesian<T,D>& rhs) {
    for (std::size_t i = 0; i < D; i++) {
        if (Util::lower  (lhs(i), rhs(i))) {
            return true;
        }
        if (Util::greater(lhs(i), rhs(i))) {
            return false;
        }
    }
    return false;
}

template<class T, std::size_t D>
bool operator<=(const Cartesian<T,D>& lhs,
                const Cartesian<T,D>& rhs) {
    return !(rhs < lhs);
}

template<class T, std::size_t D>
bool operator> (const Cartesian<T,D>& lhs,
                const Cartesian<T,D>& rhs) {
    return rhs < lhs;
}

template<class T, std::size_t D>
bool operator>=(const Cartesian<T,D>& lhs,
                const Cartesian<T,D>& rhs) {
    return !(lhs < rhs);
}

} /* namespace Vector */

namespace Util {

template<std::size_t D>
Vector::Cartesian<Real,D> round(const Vector::Cartesian<Real,D>& vec) {
    Vector::Cartesian<Real,D> res;
    for (std::size_t i = 0; i < D; i++) {
        res(i) = round(vec(i));
    }
    return  res;
}

} /* namespace Util */

} /* namespace Math */
} /* namespace SEMBA */
