#ifndef CARTESIANVECTOR_H_
#include "CartesianVector.h"
#endif

template <class T, int D>
CartesianVector<T,D>::CartesianVector() {
	for (register int i = 0; i < D; i++) {
		val[i] = T(0);
	}
}
 
template<class T, int D>
CartesianVector<T,D>::CartesianVector(T val_[D]) {
	for (register int i = 0; i < D; i++) {
		val[i] = val_[i];
	}
}

template<class T, int D>
CartesianVector<T,D>::CartesianVector(const T val_[D]) {
	for (register int i = 0; i < D; i++) {
		val[i] = val_[i];
	}
}

 
template <class T, int D>
CartesianVector<T,D>::CartesianVector(const T x, const T y, const T z) {
	assert(D==3);
	val[0] = x;
	val[1] = y;
	val[2] = z;
}

template <class T, int D>
CartesianVector<T,D>::CartesianVector(
 const CartesianVector<double,D>& begin,
 const CartesianVector<double,D>& end) {
	for (register int i = 0; i < D; i++) {
		val[i] = end.val[i]-begin.val[i];
	}
}

template<class T, int D>
CartesianVector<T,D>::~CartesianVector() {

}

template <class T, int D>
CartesianVector<T,D>&
CartesianVector<T,D>::operator=(
 const CartesianVector<double,D>& param) {
	for (register int i = 0; i < D; i++) {
		val[i] = (T) param.val[i];
	}
	return *this;
}
 
template <class T, int D>
CartesianVector<T,D>&
CartesianVector<T,D>::operator=(
 const CartesianVector<float,D>& param) {
	for (register int i = 0; i < D; i++) {
		val[i] = (T) param.val[i];
	}
	return *this;
}
 
template <class T, int D>
CartesianVector<T,D>&
CartesianVector<T,D>::operator=(
 const CartesianVector<int,D>& param) {
	for (register int i = 0; i < D; i++) {
		val[i] = (T) param.val[i];
	}
	return *this;
}
 
template <class T, int D>
CartesianVector<T,D>&
CartesianVector<T,D>::operator=(const T param) {
	for (register int i = 0; i < D; i++) {
		val[i] = param;
	}
	return *this;
}
 
template <class T, int D>
inline T&
CartesianVector<T,D>::operator() (unsigned int pos) {
	assert(pos >= 0 && pos < D);
	return val[pos];
}
 
template <class T, int D>
inline T
CartesianVector<T,D>::operator() (unsigned int pos) const {
	assert(pos >= 0 && pos < D);
	return val[pos];
}
 
template <class T, int D>
inline CartesianVector<T,D>&
CartesianVector<T,D>::operator+=(
 const CartesianVector<T,D>& param) {
	for (unsigned int i = 0; i < D; i++) {
		val[i] += param.val[i];
	}
	return *this;
}
 
template <class T, int D>
CartesianVector<T,D>&
CartesianVector<T,D>::operator+=(const T param ) {
	for (unsigned int i = 0; i < D; i++) {
		val[i] += param;
	}
	return *this;
}
 
template <class T, int D>
CartesianVector<T,D>&
CartesianVector<T,D>::operator-=(const CartesianVector<T,D>& param ) {
	for (unsigned int i = 0; i < D; i++) {
		val[i] -= param.val[i];
	}
	return *this;
}
 
template <class T, int D>
inline CartesianVector<T,D>&
CartesianVector<T,D>::operator*=(const T param) {
	for (register int i = 0; i < D; i++) {
		val[i] *= param;
	}
	return *this;
}
 
template <class T, int D>
inline CartesianVector<T,D>&
CartesianVector<T,D>::operator/=(const T param) {
	for (register int i = 0; i < D; i++) {
		val[i] /= param;
	}
	return *this;
}

template <class T, int D>
inline T
CartesianVector<T,D>::dot(
 const CartesianVector<T,D>& param) const {
	T res = 0.0;
	for (register int i = 0; i < D; i++) {
		res += val[i] * param.val[i];
	}
	return res;
}

template <class T, int D>
CartesianVector<T,D>&
CartesianVector<T,D>::normalize() {
	T nor = norm();
	for (unsigned int i = 0; i < D; i++) {
		val[i] /= nor;
	}
	return *this;
}
 
template <class T, int D>
inline double
CartesianVector<T,D>::norm() const {
	double sum = 0;
	for (unsigned int i = 0; i < D; i++) {
		sum += (double) std::abs(val[i]) * std::abs(val[i]);
	}
	return sqrt(sum);
}
 
template <class T, int D>
CartesianVector<T,D>
operator-(
 const T& lhs,
 const CartesianVector<T,D>& rhs) {
	CartesianVector<double,D> res;
	for (int i = 0; i < D; i++) {
		res.val[i] = lhs - rhs.val[i];
	}
	return res;
}

template <class T, int D>
CartesianVector<T,D>
CartesianVector<T,D>::operator+(
 const CartesianVector<T,D>& param) const {
	CartesianVector<T,D> res;
	for (register int i = 0; i < D; i++) {
		res.val[i] = val[i] + param.val[i];
	}
	return res;
}

template <class T, int D>
CartesianVector<T,D>
CartesianVector<T,D>::operator+(
 const T& param) const {
    CartesianVector<T,D> res;
    for (register int i = 0; i < D; i++) {
        res.val[i] = val[i] + param;
    }
    return res;
}
 
template <class T, int D>
CartesianVector<T,D>&
CartesianVector<T,D>::operator-() {
	for (register int i = 0; i < D; i++) {
		val[i] = - val[i];
	}
	return *this;
}
 
template <class T, int D>
CartesianVector<T,D>
CartesianVector<T,D>::operator-(
 const CartesianVector<T,D>& param) const {
	CartesianVector<T,D> res;
	for (register int i = 0; i < D; i++) {
		res.val[i] = val[i] - param.val[i];
	}
	return res;
}
 
template <class T, int D>
CartesianVector<T,D>
CartesianVector<T,D>::operator-(const T param) const {
	CartesianVector<T,D> res;
	for (int i = 0; i<D; i++) {
		res.val[i] = val[i] - param;
	}
	return res;
}
 
template <class T, int D>
CartesianVector<T,D>
CartesianVector<T,D>::operator*(const T param) const {
	CartesianVector<T, D>  res;
	for (register int i = 0; i < D; i++) {
		res.val[i] = val[i] * param;
	}
	return res;
}

template <class T, int D>
CartesianVector<T,D>
CartesianVector<T,D>::operator/(const T param) const {
	CartesianVector<T, D>  res;
	for (register int i = 0; i < D; i++) {
		res.val[i] = val[i] / param;
	}
	return res;
}
 
template <class T, int D> inline
CartesianVector<T,D>
CartesianVector<T,D>::operator*(
 const CartesianVector<T,D>& param) const {
	CartesianVector<T, D>  res;
	for (register int i = 0; i < D; i++) {
		res.val[i] = val[i] * param.val[i];
	}
	return res;
}
 
template <class T, int D>
CartesianVector<T,D>
CartesianVector<T,D>::operator^(
 const CartesianVector<T,D>& param) const {
	// PURPOSE: Computes vectorial product.
	assert(D == 3);
	CartesianVector<T,D> res;
	res.val[0] = val[1] * param.val[2] - param.val[1] * val[2];
	res.val[1] = val[2] * param.val[0] - param.val[2] * val[0];
	res.val[2] = val[0] * param.val[1] - param.val[0] * val[1];
	return res;
}

template <class T, int D> inline
bool
CartesianVector<T,D>::operator==(
 const CartesianVector<T, D>& param) const {
	for (int i = 0; i < D; i++) {
		if (std::abs(val[i] - param.val[i]) >= tolerance) {
			return false;
		}
	}
	return true;
}

template <class T, int D> inline
bool
CartesianVector<T,D>::operator!=(
 const CartesianVector<T,D>& param) const {
	return !(*this == param);
}

template <class T, int D> inline
CartesianVector<T,D>&
CartesianVector<T,D>::abs() {
	for (int i = 0; i < D; i++) {
		val[i] = (T) std::abs(val[i]);
	}
	return *this;
}

template <class T, int D> inline
CartesianVector<T,D>&
CartesianVector<T,D>::setPlusInfty() {
	for (int i = 0; i < D; i++) {
		val[i] = numeric_limits<T>::infinity();
	}
	return *this;
}

template <class T, int D> inline
CartesianVector<T,D>&
CartesianVector<T,D>::setMinusInfty() {
	for (int i = 0; i < D; i++) {
		val[i] = - numeric_limits<T>::infinity();
	}
	return *this;
}

template <class T, int D>
bool
CartesianVector<T,D>::isContainedInPlane(
 const CartesianPlane plane) const {
	assert(D == 3);
	switch (plane) {
	case xy:
		if (std::abs(val[2]) < numeric_limits<double>::epsilon()*1e8) {
			return true;
		}
		break;
	case yz:
		if (std::abs(val[0]) < numeric_limits<double>::epsilon()*1e8) {
			return true;
		}
		break;
	case zx:
		if (std::abs(val[1]) < numeric_limits<double>::epsilon()*1e8) {
			return true;
		}
		break;
	}
	return false;
}


template <class T, int D>
bool
CartesianVector<T,D>::isContainedInPlane() const {
	return (this->isContainedInPlane(xy)
	 || this->isContainedInPlane(yz) || this->isContainedInPlane(zx));
}

template<class T, int D>
inline bool
CartesianVector<T,D>::isCoplanar(
		const CartesianVector<T, D>& param) const {
	return (*this - param).isContainedInPlane();
}

template <class T, int D>
void
CartesianVector<T,D>::printInfo() const {
	cout << toStr() << flush;
}

template <class T, int D>
string
CartesianVector<T,D>::toStr() const {
   stringstream ss;
   ss << "(";
   for (register int i = 0; i < D; i++) {
      ss << val[i];
      if (i < D-1) {
         ss << " , ";
      }
   }
   ss << ")";
   return ss.str();
}

template<unsigned int VS>
CartesianVector<double,VS>
 operator/(const CartesianVector<int,VS>& lhs, const double rhs) {
	CartesianVector<double,VS> res;
	for (unsigned int i = 0; i < VS; i++) {
		res(i) = (double) lhs(i) / rhs;
	}
	return  res;
}
