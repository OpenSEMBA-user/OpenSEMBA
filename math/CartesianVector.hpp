#include "CartesianVector.h"

template <class T, Int D>
CartesianVector<T,D>::CartesianVector() {
    for (Int i = 0; i < D; i++) {
        val[i] = T(0);
    }
}

template <class T, Int D>
CartesianVector<T,D>::CartesianVector(const T val_) {
    for (Int i = 0; i < D; i++) {
        val[i] = val_;
    }
}

template<class T, Int D>
CartesianVector<T,D>::CartesianVector(T val_[D]) {
    for (Int i = 0; i < D; i++) {
        val[i] = val_[i];
    }
}

template<class T, Int D>
CartesianVector<T,D>::CartesianVector(const T val_[D]) {
    for (Int i = 0; i < D; i++) {
        val[i] = val_[i];
    }
}


template <class T, Int D>
CartesianVector<T,D>::CartesianVector(const T x, const T y, const T z) {
    assert(D==3);
    val[0] = x;
    val[1] = y;
    val[2] = z;
}

template <class T, Int D>
CartesianVector<T,D>::CartesianVector(const CartesianVector<T,D>& begin,
                                      const CartesianVector<T,D>& end) {
    for (Int i = 0; i < D; i++) {
        val[i] = end.val[i]-begin.val[i];
    }
}

template <class T, Int D> template<class U>
CartesianVector<T,D>::CartesianVector(const CartesianVector<U,D>& param) {
    for (Int i = 0; i < D; i++) {
        val[i] = (T) param.val[i];
    }
}

template<class T, Int D>
CartesianVector<T,D>::~CartesianVector() {

}

template <class T, Int D>
CartesianVector<T,D>& CartesianVector<T,D>::operator=(const T param) {
    for (Int i = 0; i < D; i++) {
        val[i] = param;
    }
    return *this;
}

template <class T, Int D> template<class U>
CartesianVector<T,D>& CartesianVector<T,D>::operator=(
        const CartesianVector<U,D>& param) {
    for (Int i = 0; i < D; i++) {
        val[i] = (T) param.val[i];
    }
    return *this;
}
//
//template <class T, Int D>
//CartesianVector<T,D>& CartesianVector<T,D>::operator=(
//        const CartesianVector<Int,D>& param) {
//    for (Int i = 0; i < D; i++) {
//        val[i] = (T) param.val[i];
//    }
//    return *this;
//}

template <class T, Int D>
CartesianVector<T,D>& CartesianVector<T,D>::operator+=(const T param) {
    for (UInt i = 0; i < D; i++) {
        val[i] += param;
    }
    return *this;
}

template <class T, Int D>
inline CartesianVector<T,D>& CartesianVector<T,D>::operator+=(
        const CartesianVector<T,D>& param) {
    for (UInt i = 0; i < D; i++) {
        val[i] += param.val[i];
    }
    return *this;
}

template <class T, Int D>
CartesianVector<T,D>& CartesianVector<T,D>::operator-=(const T param) {
    for (UInt i = 0; i < D; i++) {
        val[i] -= param;
    }
    return *this;
}

template <class T, Int D>
CartesianVector<T,D>& CartesianVector<T,D>::operator-=(
        const CartesianVector<T,D>& param) {
    for (UInt i = 0; i < D; i++) {
        val[i] -= param.val[i];
    }
    return *this;
}

template <class T, Int D>
inline CartesianVector<T,D>& CartesianVector<T,D>::operator*=(const T param) {
    for (Int i = 0; i < D; i++) {
        val[i] *= param;
    }
    return *this;
}

template <class T, Int D>
inline CartesianVector<T,D>& CartesianVector<T,D>::operator/=(const T param) {
    for (Int i = 0; i < D; i++) {
        val[i] /= param;
    }
    return *this;
}

template <class T, Int D>
CartesianVector<T,D> CartesianVector<T,D>::operator+(
        const CartesianAxis param) const {
    CartesianVector<T,D> res = *this;
    res(param)++;
    return res;
}

template <class T, Int D>
CartesianVector<T,D> CartesianVector<T,D>::operator+(const T param) const {
    CartesianVector<T,D> res;
    for (Int i = 0; i < D; i++) {
        res.val[i] = val[i] + param;
    }
    return res;
}

template <class T, Int D>
CartesianVector<T,D> CartesianVector<T,D>::operator+(
        const CartesianVector<T,D>& param) const {
    CartesianVector<T,D> res;
    for (Int i = 0; i < D; i++) {
        res.val[i] = val[i] + param.val[i];
    }
    return res;
}

template <class T, Int D>
CartesianVector<T,D>& CartesianVector<T,D>::operator-() {
    for (Int i = 0; i < D; i++) {
        val[i] = - val[i];
    }
    return *this;
}

template <class T, Int D>
CartesianVector<T,D> CartesianVector<T,D>::operator-(
        const CartesianVector<T,D>& param) const {
    CartesianVector<T,D> res;
    for (Int i = 0; i < D; i++) {
        res.val[i] = val[i] - param.val[i];
    }
    return res;
}

template <class T, Int D>
CartesianVector<T,D> CartesianVector<T,D>::operator-(const T param) const {
    CartesianVector<T,D> res;
    for (Int i = 0; i<D; i++) {
        res.val[i] = val[i] - param;
    }
    return res;
}

template <class T, Int D>
CartesianVector<T,D> operator-(const T& lhs,
                               const CartesianVector<T,D>& rhs) {
    CartesianVector<Real,D> res;
    for (Int i = 0; i < D; i++) {
        res.val[i] = lhs - rhs.val[i];
    }
    return res;
}

template <class T, Int D>
CartesianVector<T,D> CartesianVector<T,D>::operator*(const T param) const {
    CartesianVector<T, D>  res;
    for (Int i = 0; i < D; i++) {
        res.val[i] = val[i] * param;
    }
    return res;
}

template <class T, Int D> inline
CartesianVector<T,D> CartesianVector<T,D>::operator*(
        const CartesianVector<T,D>& param) const {
    CartesianVector<T, D>  res;
    for (Int i = 0; i < D; i++) {
        res.val[i] = val[i] * param.val[i];
    }
    return res;
}

template <class T, Int D>
CartesianVector<T,D> CartesianVector<T,D>::operator/(const T param) const {
    CartesianVector<T, D>  res;
    for (Int i = 0; i < D; i++) {
        res.val[i] = val[i] / param;
    }
    return res;
}

template <class T, Int D>
CartesianVector<T,D> CartesianVector<T,D>::operator^(
        const CartesianVector<T,D>& param) const {
    // PURPOSE: Computes vectorial product.
    assert(D == 3);
    CartesianVector<T,D> res;
    res.val[0] = val[1] * param.val[2] - param.val[1] * val[2];
    res.val[1] = val[2] * param.val[0] - param.val[2] * val[0];
    res.val[2] = val[0] * param.val[1] - param.val[0] * val[1];
    return res;
}

template <class T, Int D>
inline T CartesianVector<T,D>::dot(const CartesianVector<T,D>& param) const {
    T res = 0.0;
    for (Int i = 0; i < D; i++) {
        res += val[i] * param.val[i];
    }
    return res;
}

template <class T, Int D>
inline T CartesianVector<T,D>::getMax() const {
    T res = val[0];
    for (Int i = 1; i < D; i++) {
        if (val[i] > res) {
            res = val[i];
        }
    }
    return res;
}

template <class T, Int D>
CartesianVector<T,D>& CartesianVector<T,D>::setAsBinary(const UInt number) {
    assert(number < pow(2,D));
    Int den = 1;
    for (Int d = 0; d < D; d++) {
        val[D-d] = (number / den) % 2;
        den *= 2;
    }
    return *this;
}

template <class T, Int D>
bool CartesianVector<T,D>::operator==(
        const CartesianVector<T, D>& param) const {
    return MathUtils::equal((*this-param).norm(), 0.0, (*this+param).norm());
}

template <class T, Int D>
inline bool CartesianVector<T,D>::operator!=(
        const CartesianVector<T,D>& param) const {
    return !(*this == param);
}

template <class T, Int D>
bool CartesianVector<T,D>::isContainedInPlane() const {
    return (this->isContainedInPlane(xy) ||
            this->isContainedInPlane(yz) ||
            this->isContainedInPlane(zx));
}

template <class T, Int D>
bool CartesianVector<T,D>::isContainedInPlane(
        const CartesianPlane plane) const {
    assert(D == 3);
    if (is_same<T, complex<Real> >::value) {
        switch (plane) {
        case xy:
            if (MathUtils::equal(std::abs(val[2]), 0.0)) {
                return true;
            }
            break;
        case yz:
            if (MathUtils::equal(std::abs(val[0]), 0.0)) {
                return true;
            }
            break;
        case zx:
            if (MathUtils::equal(std::abs(val[1]), 0.0)) {
                return true;
            }
            break;
        }
        return false;
    } else {
        switch (plane) {
        case xy:
            if (MathUtils::equal(std::fabs(val[2]), 0.0)) {
                return true;
            }
            break;
        case yz:
            if (MathUtils::equal(std::fabs(val[0]), 0.0)) {
                return true;
            }
            break;
        case zx:
            if (MathUtils::equal(std::fabs(val[1]), 0.0)) {
                return true;
            }
            break;
        }
        return false;
    }
}

template<class T, Int D>
inline bool CartesianVector<T,D>::isCoplanar(
        const CartesianVector<T, D>& param) const {
    return (*this - param).isContainedInPlane();
}

template <class T, Int D>
inline T& CartesianVector<T,D>::operator() (UInt pos) {
    assert(pos >= 0 && pos < D);
    return val[pos];
}

template <class T, Int D>
inline T CartesianVector<T,D>::operator() (UInt pos) const {
    assert(pos >= 0 && pos < D);
    return val[pos];
}

template <class T, Int D>
inline T& CartesianVector<T,D>::operator[] (UInt pos) {
    assert(pos >= 0 && pos < D);
    return val[pos];
}

template <class T, Int D>
inline T CartesianVector<T,D>::operator[] (UInt pos) const {
    assert(pos >= 0 && pos < D);
    return val[pos];
}

template <class T, Int D>
inline Real CartesianVector<T,D>::norm() const {
    Real sum = 0;
    for (UInt i = 0; i < D; i++) {
        sum += (Real) std::fabs(val[i]) * std::fabs(val[i]);
    }
    return sqrt(sum);
}

template <class T, Int D> inline
CartesianVector<T,D>& CartesianVector<T,D>::abs() {
    for (Int i = 0; i < D; i++) {
        val[i] = (T) std::fabs(val[i]);
    }
    return *this;
}

template <class T, Int D>
CartesianVector<T,D>& CartesianVector<T,D>::normalize() {
    T nor = norm();
    for (UInt i = 0; i < D; i++) {
        val[i] /= nor;
    }
    return *this;
}

template <class T, Int D> inline
CartesianVector<T,D>& CartesianVector<T,D>::setPlusInfty() {
    for (Int i = 0; i < D; i++) {
        if (numeric_limits<T>::has_infinity) {
            val[i] = numeric_limits<T>::infinity();
        } else {
            val[i] = numeric_limits<T>::max();
        }
    }
    return *this;
}

template <class T, Int D> inline
CartesianVector<T,D>& CartesianVector<T,D>::setMinusInfty() {
    for (Int i = 0; i < D; i++) {
        if (numeric_limits<T>::has_infinity) {
            val[i] = - numeric_limits<T>::infinity();
        } else {
            val[i] = numeric_limits<T>::min();
        }
    }
    return *this;
}

template <class T, Int D>
string CartesianVector<T,D>::toStr() const {
    stringstream ss;
    ss << "(";
    for (Int i = 0; i < D; i++) {
        ss << val[i];
        if (i < D-1) {
            ss << " , ";
        }
    }
    ss << ")";
    return ss.str();
}

template <class T, Int D>
void CartesianVector<T,D>::printInfo() const {
    cout << toStr() << flush;
}

template<Int D>
CartesianVector<Real,D> operator+(const CartesianVector<Int,D> & lhs,
                                  const CartesianVector<Real,D>& rhs) {
    CartesianVector<Real,D> res;
    for (UInt i = 0; i < D; i++) {
        res(i) = lhs(i) + rhs(i);
    }
    return res;
}

template<Int D>
CartesianVector<Real,D> operator/(const CartesianVector<Int,D>& lhs,
                                  const Real rhs) {
    CartesianVector<Real,D> res;
    for (Int i = 0; i < D; i++) {
        res(i) = (Real) lhs(i) / rhs;
    }
    return  res;
}

template<class T, Int D>
bool operator< (const CartesianVector<T,D>& lhs,
                const CartesianVector<T,D>& rhs) {
    for (Int i = 0; i < D; i++) {
        if (MathUtils::lower   (lhs(i), rhs(i))) {
            return true;
        }
        if (MathUtils::greater(lhs(i), rhs(i))) {
            return false;
        }
    }
    return false;
}

template<class T, Int D>
bool operator<=(const CartesianVector<T,D>& lhs,
                const CartesianVector<T,D>& rhs) {
    return !(rhs < lhs);
}

template<class T, Int D>
bool operator> (const CartesianVector<T,D>& lhs,
                const CartesianVector<T,D>& rhs) {
    return rhs < lhs;
}

template<class T, Int D>
bool operator>=(const CartesianVector<T,D>& lhs,
                const CartesianVector<T,D>& rhs) {
    return !(lhs < rhs);
}

template<Int D>
CartesianVector<Real,D> MathUtils::round(const CartesianVector<Real,D>& vec) {
    CartesianVector<Real,D> res;
    for (UInt i = 0; i < D; i++) {
        res(i) = round(vec(i));
    }
    return  res;
}
