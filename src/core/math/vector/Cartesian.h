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

#ifndef SEMBA_MATH_VECTOR_CARTESIAN_H_
#define SEMBA_MATH_VECTOR_CARTESIAN_H_

#include <iostream>
#include <complex>
#include <stdexcept>

#include "math/Types.h"
#include "math/Constants.h"
#include "math/util/Real.h"

namespace SEMBA {
namespace Math {
namespace Vector {

template <class T, std::size_t D>
class Cartesian {
public:
    T val[D];
    Cartesian();
    Cartesian<T,D>(const T val_);
    Cartesian<T,D>(T val_[D]);
    Cartesian<T,D>(const T val_[D]);
    Cartesian<T,D>(const T, const T, const T);
    Cartesian<T,D>(const Cartesian<T,D>&,
                   const Cartesian<T,D>&);
    template<class U>
    Cartesian<T,D>(const Cartesian<U,D>&);
    virtual ~Cartesian();

    Cartesian<T,D>& operator= (const T);

    template<class U>
    Cartesian<T,D>& operator= (const Cartesian<U,D>&);

    Cartesian<T,D>& operator+=(const T param);
    Cartesian<T,D>& operator+=(const Cartesian<T,D>&);
    Cartesian<T,D>& operator-=(const T param);
    Cartesian<T,D>& operator-=(const Cartesian<T,D>&);
    Cartesian<T,D>& operator*=(const T param);
    Cartesian<T,D>& operator/=(const T param);

    Cartesian<T,D>  operator+(const T param) const;
    Cartesian<T,D>  operator+(const Cartesian<T,D>& param) const;
    Cartesian<T,D>& operator-();
    Cartesian<T,D>  operator-(const T param) const;
    Cartesian<T,D>  operator-(const Cartesian<T,D>& param) const;
    Cartesian<T,D>  operator*(const T param) const;
    Cartesian<T,D>  operator*(const Cartesian<T,D>& param) const;
    Cartesian<T,D>  operator/(const T param) const;
    Cartesian<T,D>  operator^(const Cartesian<T,D>& param) const;

    T dot(const Cartesian<T,D>& param) const;
    T getMax() const;

    bool operator==(const Cartesian<T,D>& param) const;
    bool operator!=(const Cartesian<T,D>& param) const;
    bool isContainedInPlane() const;
    bool isContainedInPlane(
            const Constants::CartesianPlane plane) const;
    bool isCoplanar(const Cartesian<T,D>& param) const;

    bool                     isInCartesianAxis() const;
    Constants::CartesianAxis getCartesianAxis() const;

    T& operator() (std::size_t pos);
    T  operator() (std::size_t pos) const;

    T& operator[] (std::size_t pos);
    T  operator[] (std::size_t pos) const;

    Cartesian<T,D>& setAsBinary(std::size_t number);
    Cartesian<T,D>& setWithMinimalComponents(const Cartesian<T,D>& rhs);

    Real norm() const;

    Cartesian<T,D>& abs();
    Cartesian<T,D>& normalize();
    Cartesian<T,D>& setPlusInfty();
    Cartesian<T,D>& setMinusInfty();

    Cartesian<T,D>& cyclicPermutation(const std::size_t n=1);

    std::string toStr() const;
    void printInfo() const;
};

template<std::size_t D>
Cartesian<Real,D> operator+(const Cartesian<Int ,D>& lhs,
                            const Cartesian<Real,D>& rhs);
template<std::size_t D>
Cartesian<Real,D> operator/(const Cartesian<Int,D>& lhs,
                            const Real rhs);
template<class T, std::size_t D>
bool operator< (const Cartesian<T,D>& lhs,
                const Cartesian<T,D>& rhs);
template<class T, std::size_t D>
bool operator<=(const Cartesian<T,D>& lhs,
                const Cartesian<T,D>& rhs);
template<class T, std::size_t D>
bool operator> (const Cartesian<T,D>& lhs,
                const Cartesian<T,D>& rhs);
template<class T, std::size_t D>
bool operator>=(const Cartesian<T,D>& lhs,
                const Cartesian<T,D>& rhs);

template <class T, std::size_t D>
std::ostream& operator<<(std::ostream& os, const Cartesian<T,D>& vec) {
    return os << vec.toStr();
}

} /* namespace Vector */

namespace Util {

template<std::size_t D>
Vector::Cartesian<Real,D> round(const Vector::Cartesian<Real,D>& vec);

} /* namespace Util */

} /* namespace Math */
} /* namespace SEMBA */

#include "Cartesian.hpp"

namespace SEMBA {
namespace Math {

typedef Vector::Cartesian<Real,2> CVecR2;
typedef Vector::Cartesian<Real,3> CVecR3;
typedef Vector::Cartesian<Real,4> CVecR4;
typedef Vector::Cartesian<Int ,2> CVecI2;
typedef Vector::Cartesian<Int ,3> CVecI3;

typedef Vector::Cartesian<std::complex<Real>,3> CVecC3;

} /* namespace Math */
} /* namespace SEMBA */

#endif /* SEMBA_MATH_VECTOR_CARTESIAN_H_ */
