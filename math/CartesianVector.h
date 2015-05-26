#ifndef CARTESIANVECTOR_H_
#define CARTESIANVECTOR_H_

#include <cassert>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <type_traits>
using namespace std;

#include "RealUtils.h"

typedef enum {
	xy = 0,
	yz = 1,
	zx = 2
} CartesianPlane;

typedef enum {
	x = 0,
	y = 1,
	z = 2
} CartesianAxis;

typedef enum {
    L = 0,
    U = 1
} CartesianBound;

template <class T, Int D>
class CartesianVector {
public:
    T val[D];
    CartesianVector();
    CartesianVector<T,D>(const T val_);
    CartesianVector<T,D>(T val_[D]);
    CartesianVector<T,D>(const T val_[D]);
    CartesianVector<T,D>(const T, const T, const T);
    CartesianVector<T,D>(const CartesianVector<T,D>&,
                         const CartesianVector<T,D>&);
    template<class U>
    CartesianVector<T,D>(const CartesianVector<U,D>&);
    virtual ~CartesianVector();

    CartesianVector<T,D>& operator= (const T);

    template<class U>
    CartesianVector<T,D>& operator= (const CartesianVector<U,D>&);

//    CartesianVector<T,D>& operator= (const CartesianVector<Int ,D>&);
//
    CartesianVector<T,D>& operator+=(const T param);
    CartesianVector<T,D>& operator+=(const CartesianVector<T,D>&);
    CartesianVector<T,D>& operator-=(const T param);
    CartesianVector<T,D>& operator-=(const CartesianVector<T,D>&);
    CartesianVector<T,D>& operator*=(const T param);
    CartesianVector<T,D>& operator/=(const T param);

    CartesianVector<T,D>  operator+(const CartesianAxis param) const;
    CartesianVector<T,D>  operator+(const T param) const;
    CartesianVector<T,D>  operator+(const CartesianVector<T,D>& param) const;
    CartesianVector<T,D>& operator-();
    CartesianVector<T,D>  operator-(const T param) const;
    CartesianVector<T,D>  operator-(const CartesianVector<T,D>& param) const;
    CartesianVector<T,D>  operator*(const T param) const;
    CartesianVector<T,D>  operator*(const CartesianVector<T,D>& param) const;
    CartesianVector<T,D>  operator/(const T param) const;
    CartesianVector<T,D>  operator^(const CartesianVector<T,D>& param) const;

    T dot(const CartesianVector<T,D>& param) const;
    T getMax() const;

    virtual bool operator==(const CartesianVector<T,D>& param) const;
    virtual bool operator!=(const CartesianVector<T,D>& param) const;
    virtual bool isContainedInPlane() const;
    virtual bool isContainedInPlane(const CartesianPlane plane) const;
    virtual bool isCoplanar(const CartesianVector<T,D>& param) const;

    virtual T& operator() (UInt pos);
    virtual T  operator() (UInt pos) const;

    virtual T& operator[] (UInt pos);
    virtual T  operator[] (UInt pos) const;

    virtual Real norm() const;

    CartesianVector<T,D>& abs();
    CartesianVector<T,D>& normalize();
    CartesianVector<T,D>& setPlusInfty();
    CartesianVector<T,D>& setMinusInfty();

    virtual string toStr() const;
    virtual void printInfo() const;
};

template<Int D>
CartesianVector<Real,D> operator+(const CartesianVector<Int ,D>& lhs,
                                  const CartesianVector<Real,D>& rhs);
template<Int D>
CartesianVector<Real,D> operator/(const CartesianVector<Int,D>& lhs,
                                  const Real rhs);
template<class T, Int D>
bool operator< (const CartesianVector<T,D>& lhs,
                const CartesianVector<T,D>& rhs);
template<class T, Int D>
bool operator<=(const CartesianVector<T,D>& lhs,
                const CartesianVector<T,D>& rhs);
template<class T, Int D>
bool operator> (const CartesianVector<T,D>& lhs,
                const CartesianVector<T,D>& rhs);
template<class T, Int D>
bool operator>=(const CartesianVector<T,D>& lhs,
                const CartesianVector<T,D>& rhs);

template <class T, Int D>
std::ostream& operator<<(ostream& os, const CartesianVector<T,D>& vec) {
   return os << vec.toStr();
}

namespace MathUtils {

template<Int D>
CartesianVector<Real,D> round(const CartesianVector<Real,D>& vec);

}

#include "CartesianVector.hpp"

typedef CartesianVector<Real,2> CVecR2;
typedef CartesianVector<Real,3> CVecR3;
typedef CartesianVector<Int ,2> CVecI2;
typedef CartesianVector<Int ,3> CVecI3;

typedef CartesianVector<complex<Real>,3> CVecC3;

#endif /* MATHVECTOR_H_ */
