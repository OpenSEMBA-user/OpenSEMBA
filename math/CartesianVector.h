#ifndef CARTESIANVECTOR_H_
#define CARTESIANVECTOR_H_

#include <iostream>
#include <cmath>
#include <assert.h>
#include <complex>
#include <limits>
#include <type_traits>

using namespace std;

#ifndef CARTESIANVECTOR_ERROR
#define CARTESIANVECTOR_ERROR 677722
#endif

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

#include "Types.h"

template <class T, Int D>
class CartesianVector {
public:
	T val[D];
	static Real tolerance;
	CartesianVector();
	virtual ~CartesianVector();
	CartesianVector<T,D>(T val_[D]);
	CartesianVector<T,D>(const T val_[D]);
	CartesianVector<T,D>(const T, const T, const T);
    CartesianVector<T,D>(const CartesianVector<Real,D>& begin,
						 const CartesianVector<Real,D>& end);
	virtual CartesianVector<T,D>&
	 operator=(const CartesianVector<Real,D>& param);
	virtual CartesianVector<T,D>&
	 operator=(const CartesianVector<Int,D>& param);
	virtual CartesianVector<T,D>&
	 operator=(const T);
	virtual T&
	 operator() (UInt pos);
	virtual T
	 operator() (UInt pos) const;
	virtual CartesianVector<T,D>&
	 operator+=(const CartesianVector<T,D>& param);
	virtual CartesianVector<T,D>&
	 operator+=(const T param);
	virtual CartesianVector<T,D>&
	 operator-=(const CartesianVector<T,D>& param);
	virtual CartesianVector<T,D>&
	 operator*=(const T param);
	virtual CartesianVector<T,D>&
	 operator/=(const T param);
	virtual CartesianVector<T,D>&
	 normalize();
	virtual Real
	 norm() const;
	virtual T
	 dot(const CartesianVector<T,D>& param) const;
	virtual CartesianVector<T,D>
	 operator+(const CartesianVector<T,D>& param) const;
    virtual CartesianVector<T,D>
     operator+(const T& param) const;
	virtual CartesianVector<T,D>&
	 operator-();
	virtual CartesianVector<T,D>
	 operator-(const CartesianVector<T,D>& param) const;
	virtual CartesianVector<T,D>
	 operator-(const T param) const;
	virtual CartesianVector<T,D>
	 operator*(const T param) const;
	virtual CartesianVector<T,D>
	 operator/(const T param) const;
	virtual CartesianVector<T,D>
	 operator*(const CartesianVector<T,D>& param) const;
	virtual CartesianVector<T,D>
	 operator^(const CartesianVector<T,D>& param) const;
	virtual CartesianVector<T,D>&
	 abs();
	virtual CartesianVector<T,D>&
	 setPlusInfty();
	virtual CartesianVector<T,D>&
	 setMinusInfty();
	virtual bool
	 operator==(const CartesianVector<T,D>& param) const;
	virtual bool
	 operator!=(const CartesianVector<T,D>& param) const;
	virtual bool
	 isContainedInPlane(
	  const CartesianPlane plane) const;
	virtual bool
	 isContainedInPlane() const;
	virtual bool
	 isCoplanar(const CartesianVector<T,D>& param) const;
	virtual void
	 printInfo() const;
	virtual string
	 toStr() const;
};

template<class T, Int D>
Real CartesianVector<T,D>::tolerance = 1.0e-10;

template<Int VS>
CartesianVector<Real,VS>
 operator/(
  const CartesianVector<Int,VS>& lhs,
  const Real rhs);

template <class T, Int D>
std::ostream&
operator<<(
      ostream& os,
      const CartesianVector<T,D>& vec) {
   return os << vec.toStr();
}

#include "CartesianVector.hpp"

template <class T, Int D>
using CVec = CartesianVector<T,D>;

typedef CartesianVector<Real,3> CVecR3;
typedef CartesianVector<Int ,2> CVecI2;
typedef CartesianVector<Int ,3> CVecI3;

typedef CartesianVector<complex<Real>,3> CVecC3;

#endif /* MATHVECTOR_H_ */
