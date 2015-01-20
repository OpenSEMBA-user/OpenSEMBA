#ifndef CARTESIANVECTOR_H_
#define CARTESIANVECTOR_H_

#include <iostream>
#include <cmath>
#include <assert.h>
#include <complex>
#include <limits>
#include <type_traits>

using namespace std;

////typedef unsigned int uint;;

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

template <class T, int D>
class CartesianVector {
public:
	T val[D];
	static const double tolerance = 1.0e-10;
	CartesianVector();
	virtual ~CartesianVector();
	CartesianVector<T,D>(T val_[D]);
	CartesianVector<T,D>(const T val_[D]);
	CartesianVector<T,D>(const T, const T, const T);
	CartesianVector<T,D>(const CartesianVector<double,D>& begin,
						 const CartesianVector<double,D>& end);
	virtual CartesianVector<T,D>&
	 operator=(const CartesianVector<double,D>& param);
	virtual CartesianVector<T,D>&
	 operator=(const CartesianVector<float,D>& param);
	virtual CartesianVector<T,D>&
	 operator=(const CartesianVector<int,D>& param);
	virtual CartesianVector<T,D>&
	 operator=(const T);
	virtual T&
	 operator() (unsigned int pos);
	virtual T
	 operator() (unsigned int pos) const;
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
	virtual double
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

template<int VS>
CartesianVector<double,VS>
 operator/(
  const CartesianVector<int,VS>& lhs,
  const double rhs);

template <class T, int D>
std::ostream&
operator<<(
      ostream& os,
      const CartesianVector<T,D>& vec) {
   return os << vec.toStr();
}

#include "CartesianVector.hpp"

typedef CartesianVector<double,3> CVecD3;
typedef CartesianVector<long int, 3> CVecI3;
typedef CartesianVector<complex<double>,3> CVecC3;

#endif /* MATHVECTOR_H_ */
