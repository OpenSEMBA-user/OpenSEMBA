

#pragma once

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
    // TODO: Remove plain array
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
    Cartesian<T,D>& operator*=(const Cartesian<T,D>&);
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

