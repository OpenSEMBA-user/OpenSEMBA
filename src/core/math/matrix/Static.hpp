

#include "Static.h"

namespace SEMBA {
namespace Math {
namespace Matrix {

template <class T, std::size_t ROWS, std::size_t COLS>
Static<T,ROWS,COLS>::Static() {
    for (std::size_t i = 0; i < ROWS*COLS; i++) {
        _val[i] = T(0);
    }
}

template <class T, std::size_t ROWS, std::size_t COLS>
Static<T,ROWS,COLS>::Static(
        const Static<T,ROWS,COLS>& param) {
    std::size_t nRnC = ROWS * COLS;
    for (std::size_t i = 0; i < nRnC; i++) {
        _val[i] = (T) param._val[i];
    }
}

template <class T, std::size_t ROWS, std::size_t COLS>
Static<T,ROWS,COLS>::~Static() {
}

template <class T, std::size_t ROWS, std::size_t COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::operator=(
        const Static<T,ROWS,COLS>& param) {
    if (this == &param)
        return *this;
    for (std::size_t i = 0; i < ROWS*COLS; i++)
        _val[i] = param._val[i];
    return *this;
}

template <class T, std::size_t ROWS, std::size_t COLS>
inline T Static<T,ROWS,COLS>::val(const std::size_t i) const {
    return _val[i];
}

template <class T, std::size_t ROWS, std::size_t COLS>
inline T& Static<T,ROWS,COLS>::val(const std::size_t i) {
    return _val[i];
}

template <class T, std::size_t ROWS, std::size_t COLS>
inline T Static<T,ROWS,COLS>::val(const std::size_t row,
                                  const std::size_t col) const {
    return _val[row * COLS + col];
}

template <class T, std::size_t ROWS, std::size_t COLS>
inline T& Static<T,ROWS,COLS>::val(const std::size_t row, 
                                   const std::size_t col) {
    return _val[row * COLS + col];
}

template <class T, std::size_t ROWS, std::size_t COLS>
inline const T* Static<T,ROWS,COLS>::val() const {
    return _val;
}

template<class T, std::size_t ROWS, std::size_t COLS>
inline T Static<T,ROWS,COLS>::operator()(
        const std::size_t row,
        const std::size_t col) const {
    return _val[row * COLS + col];
}

template<class T, std::size_t ROWS, std::size_t COLS>
inline T& Static<T,ROWS,COLS>::operator()(const std::size_t row,
                                          const std::size_t col) {
    return _val[row * COLS + col];
}

template <class T, std::size_t ROWS, std::size_t COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::assign(
        const Static<Int,ROWS,COLS>& param) {
    for (std::size_t i = 0; i < ROWS; i++) {
        for (std::size_t j = 0; j < COLS; j++) {
            val(i,j) = param(i,j);
        }
    }
    return *this;
}

template <class T, std::size_t ROWS, std::size_t COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::setInDiagonal(
        const Vector::Cartesian<T,ROWS>& rhs) {
    static_assert(ROWS == COLS, "Requires squire matrix");
    for (std::size_t i = 0; i < ROWS; i++) {
        val(i,i) = rhs(i);
    }
    return *this;
}

template <class T, std::size_t ROWS, std::size_t COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::operator=(
        const Dynamic<Real>& param) {
    assert(ROWS == param.nRows() && COLS == param.nCols());
    for (std::size_t i = 0; i < ROWS; i++)
        for (std::size_t j = 0; j < COLS; j++)
            val(i,j) = param(i,j);
    return *this;
}

template<class T, std::size_t ROWS, std::size_t COLS>
inline Static<T, ROWS, COLS> Static<T,ROWS,COLS>::operator+(
        Static<T, ROWS, COLS>& param) const {
    Static<T,ROWS,COLS> res;
    std::size_t n = ROWS*COLS;
    for (std::size_t i = 0; i < n; i++) {
        res._val[i] = _val[i] + param._val[i];
    }
    return res;
}

template<class T, std::size_t ROWS, std::size_t COLS>
std::array<std::complex<Real>, ROWS>
        Static<T,ROWS,COLS>::getEigenvalues() const {
}

template<class T, std::size_t ROWS, std::size_t COLS>
Static<T,COLS,ROWS>&
Static<T,ROWS,COLS>::invert() {
    this->internal_();
    return *this;
}

template<class T, std::size_t ROWS, std::size_t COLS>
inline std::size_t Static<T,ROWS,COLS>::nCols() const {
    return COLS;
}

template<class T, std::size_t ROWS, std::size_t COLS>
inline std::size_t Static<T,ROWS,COLS>::nRows() const {
    return ROWS;
}

template<class T, std::size_t ROWS, std::size_t COLS>
Static<T,COLS,ROWS> Static<T,ROWS,COLS>::transpose() {
    Static<T,COLS,ROWS> res;
    for (std::size_t i = 0; i < ROWS; i++) {
        for (std::size_t j = 0; j < COLS; j++) {
            res(j,i) = val(i,j);
        }
    }
    return res;
}

template<class T, std::size_t ROWS, std::size_t COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::operator+=(
        const Real param) {
    for (std::size_t i = 0; i < ROWS*COLS; i++)
        _val[i] += param;
    return *this;
}

template<class T, std::size_t ROWS, std::size_t COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::operator+=(
        const Static<T,ROWS,COLS>& param) {
    for (std::size_t i = 0; i < ROWS*COLS; i++) {
        _val[i] += param._val[i];
    }
    return *this;
}

template<class T, std::size_t ROWS, std::size_t COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::operator-=(
        const Static<T,ROWS,COLS>& param) {
    for (std::size_t i = 0; i < ROWS*COLS; i++) {
        _val[i] -= param._val[i];
    }
    return *this;
}

template<class T, std::size_t ROWS, std::size_t COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::operator*=(const T param) {
    for (std::size_t i = 0; i < ROWS*COLS; i++) {
        _val[i] *= param;
    }
    return *this;
}

template<class T, std::size_t ROWS, std::size_t COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::operator/=(const T param) {
    for (std::size_t i = 0; i < ROWS*COLS; i++) {
        _val[i] /= param;
    }
    return *this;
}

template<class T, std::size_t ROWS, std::size_t COLS>
Static<T,ROWS,COLS> Static<T,ROWS,COLS>::operator*(T param) const {
    Static<T,ROWS,COLS> res;
    for (std::size_t i = 0; i < ROWS*COLS; i++) {
        res._val[i] = _val[i] * param;
    }
    return res;
}

template<class T, std::size_t ROWS, std::size_t COLS>
Vector::Cartesian<T,COLS> Static<T,ROWS,COLS>::operator*(
        Vector::Cartesian<T,COLS> rhs) const {
    Vector::Cartesian<T,COLS> res;
    for (std::size_t i = 0; i < ROWS; i++) {
        for (std::size_t j = 0; j < COLS; j++) {
            res(i) += this->val(i,j)*rhs(j);
        }
    }
    return res;
}

template<class T, std::size_t ROWS, std::size_t COLS>
bool
Static<T,ROWS,COLS>::operator==(
        const Static<T,ROWS,COLS>& param) const {
    for (std::size_t i = 0; i < ROWS*COLS; i++) {
        Real diff = std::abs(_val[i] -param._val[i]);
        if (diff > std::numeric_limits<Real>::epsilon() * 1e2) {
            return false;
        }
    }
    return true;
}

template<class T, std::size_t ROWS, std::size_t COLS>
bool
Static<T,ROWS,COLS>::operator<(
        const Static<T,ROWS,COLS>& param) const {
    for (std::size_t i = 0; i < (ROWS*COLS); i++) {
        if (val(i) < param.val(i)) {
            return true;
        }
        if (val(i) > param.val(i)) {
            return false;
        }
    }
    return false;
}

template<class T, std::size_t ROWS, std::size_t COLS>
Static<T,ROWS,COLS>& Static<T,ROWS,COLS>::operator=(
        const Dynamic<Int>& rhs) {
    assert(ROWS == rhs.nRows() && COLS == rhs.nCols());
    for (std::size_t i = 0; i < ROWS; i++)
        for (std::size_t j = 0; j < COLS; j++)
            val(i,j) = (T) rhs(i,j);
    return *this;
}

template<class T, std::size_t ROWS, std::size_t COLS>
void
Static<T,ROWS,COLS>::copy(std::vector<std::vector<T> > values) {
    assert(ROWS == values.size());
    for (std::size_t i = 0; i < ROWS; i++) {
        assert(values[i].size() == COLS);
        for (std::size_t j = 0; j < COLS; j++) {
            val(i,j) = values[i][j];
        }
    }
}


template<class T, class S,
         std::size_t ROWS, std::size_t COLS, std::size_t NCOLSB>
Static<T,ROWS,NCOLSB> operator*(
        const Static<T,ROWS,COLS>& lhs,
        const Static<S,COLS,NCOLSB>& rhs) {
    std::size_t i, j, k;
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

template<class T, std::size_t ROWS, std::size_t COLS>
Dynamic<T> operator*(
        const Static<T,ROWS,COLS>& lhs,
        const Dynamic<T>& rhs) {
    assert(COLS == rhs.nRows());
    std::size_t i, j, k;
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
