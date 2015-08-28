#include "StaMatrix.h"

template <class T, UInt ROWS, UInt COLS>
StaMatrix<T,ROWS,COLS>::StaMatrix() {
    for (UInt i = 0; i < ROWS*COLS; i++) {
        _val[i] = T(0);
    }
}

template <class T, UInt ROWS, UInt COLS>
StaMatrix<T,ROWS,COLS>::StaMatrix(
        const StaMatrix<T,ROWS,COLS>& param) {
    UInt nRnC = ROWS * COLS;
    for (UInt i = 0; i < nRnC; i++) {
        _val[i] = (T) param._val[i];
    }
}

template <class T, UInt ROWS, UInt COLS>
StaMatrix<T,ROWS,COLS>::~StaMatrix() {
}

template <class T, UInt ROWS, UInt COLS>
StaMatrix<T,ROWS,COLS>& StaMatrix<T,ROWS,COLS>::operator=(
        const StaMatrix<T,ROWS,COLS>& param) {
    if (this == &param)
        return *this;
    for (UInt i = 0; i < ROWS*COLS; i++)
        _val[i] = param._val[i];
    return *this;
}

template <class T, UInt ROWS, UInt COLS>
inline T StaMatrix<T,ROWS,COLS>::val(const UInt i) const {
    return _val[i];
}

template <class T, UInt ROWS, UInt COLS>
inline T& StaMatrix<T,ROWS,COLS>::val(const UInt i) {
    return _val[i];
}

template <class T, UInt ROWS, UInt COLS>
inline T StaMatrix<T,ROWS,COLS>::val(const UInt row, const UInt col) const {
    return _val[row * COLS + col];
}

template <class T, UInt ROWS, UInt COLS>
inline T& StaMatrix<T,ROWS,COLS>::val(const UInt row, const UInt col) {
    return _val[row * COLS + col];
}

template <class T, UInt ROWS, UInt COLS>
inline const T* StaMatrix<T,ROWS,COLS>::val() const {
    return _val;
}

template<class T, UInt ROWS, UInt COLS>
inline T StaMatrix<T,ROWS,COLS>::operator()(
        const UInt row,
        const UInt col) const {
    return _val[row * COLS + col];
}

template<class T, UInt ROWS, UInt COLS>
inline T& StaMatrix<T,ROWS,COLS>::operator()(const UInt row, const UInt col) {
    return _val[row * COLS + col];
}

template <class T, UInt ROWS, UInt COLS>
StaMatrix<T,ROWS,COLS>& StaMatrix<T,ROWS,COLS>::assign(
        const StaMatrix<Int,ROWS,COLS>& param) {
    for (UInt i = 0; i < ROWS; i++) {
        for (UInt j = 0; j < COLS; j++) {
            val(i,j) = param(i,j);
        }
    }
    return *this;
}

template <class T, UInt ROWS, UInt COLS>
StaMatrix<T,ROWS,COLS>& StaMatrix<T,ROWS,COLS>::setInDiagonal(
        const CartesianVector<T,ROWS>& rhs) {
    static_assert(ROWS == COLS, "Requires squire matrix");
    for (UInt i = 0; i < ROWS; i++) {
        val(i,i) = rhs(i);
    }
    return *this;
}

template <class T, UInt ROWS, UInt COLS>
StaMatrix<T,ROWS,COLS>& StaMatrix<T,ROWS,COLS>::operator=(
        const DynMatrix<Real>& param) {
    assert(ROWS == param.nRows() && COLS == param.nCols());
    for (UInt i = 0; i < ROWS; i++)
        for (UInt j = 0; j < COLS; j++)
            val(i,j) = param(i,j);
    return *this;
}

template<class T, UInt ROWS, UInt COLS>
inline StaMatrix<T, ROWS, COLS> StaMatrix<T,ROWS,COLS>::operator+(
        StaMatrix<T, ROWS, COLS>& param) const {
    StaMatrix<T,ROWS,COLS> res;
    UInt n = ROWS*COLS;
    for (UInt i = 0; i < n; i++) {
        res._val[i] = _val[i] + param._val[i];
    }
    return res;
}

template <class T, UInt ROWS, UInt COLS>
void StaMatrix<T,ROWS,COLS>::printInfo() const {
    printInfo(ROWS, COLS);
}

template <class T, UInt ROWS, UInt COLS>
void StaMatrix<T,ROWS,COLS>::printInfo(UInt rows, UInt cols) const {
    UInt i, j;
    if (rows > ROWS || cols > COLS) {
        throw typename MathMatrix<T>::ErrorSize();
    }
    cout << "Matrix type: Double";
    cout << "Dimensions: " << ROWS << "x" << COLS << endl;
    cout << "Stored values: " << endl;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++)
            cout << val(i,j) << " ";
        cout << endl;
    }
}

template<class T, UInt ROWS, UInt COLS>
StaMatrix<T,COLS,ROWS>&
StaMatrix<T,ROWS,COLS>::invert() {
    this->internalInvert();
    return *this;
}

template<class T, UInt ROWS, UInt COLS>
inline UInt StaMatrix<T,ROWS,COLS>::nCols() const {
    return COLS;
}

template<class T, UInt ROWS, UInt COLS>
inline UInt StaMatrix<T,ROWS,COLS>::nRows() const {
    return ROWS;
}

template<class T, UInt ROWS, UInt COLS>
StaMatrix<T,COLS,ROWS> StaMatrix<T,ROWS,COLS>::transpose() {
    StaMatrix<T,COLS,ROWS> res;
    for (UInt i = 0; i < ROWS; i++) {
        for (UInt j = 0; j < COLS; j++) {
            res(j,i) = val(i,j);
        }
    }
    return res;
}

template<class T, UInt ROWS, UInt COLS>
StaMatrix<T,ROWS,COLS>& StaMatrix<T,ROWS,COLS>::operator+=(
        const Real param) {
    for (UInt i = 0; i < ROWS*COLS; i++)
        _val[i] += param;
    return *this;
}

template<class T, UInt ROWS, UInt COLS>
StaMatrix<T,ROWS,COLS>& StaMatrix<T,ROWS,COLS>::operator+=(
        const StaMatrix<T,ROWS,COLS>& param) {
    for (UInt i = 0; i < ROWS*COLS; i++) {
        _val[i] += param._val[i];
    }
    return *this;
}

template<class T, UInt ROWS, UInt COLS>
StaMatrix<T,ROWS,COLS>& StaMatrix<T,ROWS,COLS>::operator-=(
        const StaMatrix<T,ROWS,COLS>& param) {
    for (UInt i = 0; i < ROWS*COLS; i++) {
        _val[i] -= param._val[i];
    }
    return *this;
}

template<class T, UInt ROWS, UInt COLS>
StaMatrix<T,ROWS,COLS>& StaMatrix<T,ROWS,COLS>::operator*=(const T param) {
    for (UInt i = 0; i < ROWS*COLS; i++) {
        _val[i] *= param;
    }
    return *this;
}

template<class T, UInt ROWS, UInt COLS>
StaMatrix<T,ROWS,COLS>& StaMatrix<T,ROWS,COLS>::operator/=(const T param) {
    for (UInt i = 0; i < ROWS*COLS; i++) {
        _val[i] /= param;
    }
    return *this;
}

template<class T, UInt ROWS, UInt COLS>
StaMatrix<T,ROWS,COLS> StaMatrix<T,ROWS,COLS>::operator*(T param) const {
    StaMatrix<T,ROWS,COLS> res;
    for (UInt i = 0; i < ROWS*COLS; i++) {
        res._val[i] = _val[i] * param;
    }
    return res;
}

template<class T, UInt ROWS, UInt COLS>
CartesianVector<T,COLS> StaMatrix<T,ROWS,COLS>::operator*(
        CartesianVector<T,COLS> rhs) const {
    CartesianVector<T,COLS> res;
    for (UInt i = 0; i < ROWS; i++) {
        for (UInt j = 0; j < COLS; j++) {
            res(i) += this->val(i,j)*rhs(j);
        }
    }
    return res;
}

template<class T, UInt ROWS, UInt COLS>
bool
StaMatrix<T,ROWS,COLS>::operator==(
        const StaMatrix<T,ROWS,COLS>& param) const {
    for (UInt i = 0; i < ROWS*COLS; i++) {
        T diff = abs(_val[i] -param._val[i]);
        if (diff > numeric_limits<T>::epsilon() * 1e2) {
            return false;
        }
    }
    return true;
}

template<class T, UInt ROWS, UInt COLS>
bool
StaMatrix<T,ROWS,COLS>::operator<(
        const StaMatrix<T,ROWS,COLS>& param) const {
    for (UInt i = 0; i < (ROWS*COLS); i++) {
        if (val(i) < param.val(i)) {
            return true;
        }
        if (val(i) > param.val(i)) {
            return false;
        }
    }
    return false;
}

template<class T, UInt ROWS, UInt COLS>
StaMatrix<T,ROWS,COLS>& StaMatrix<T,ROWS,COLS>::operator=(
        const DynMatrix<Int>& rhs) {
    assert(ROWS == rhs.nRows() && COLS == rhs.nCols());
    for (UInt i = 0; i < ROWS; i++)
        for (UInt j = 0; j < COLS; j++)
            val(i,j) = (T) rhs(i,j);
    return *this;
}

template<class T, UInt ROWS, UInt COLS>
void
StaMatrix<T,ROWS,COLS>::copy(vector<vector<T> > values) {
    assert(ROWS == values.size());
    for (UInt i = 0; i < ROWS; i++) {
        assert(values[i].size() == COLS);
        for (UInt j = 0; j < COLS; j++) {
            val(i,j) = values[i][j];
        }
    }
}


template<class T, class S, UInt ROWS, UInt COLS, UInt NCOLSB>
StaMatrix<T,ROWS,NCOLSB> operator*(
        const StaMatrix<T,ROWS,COLS>& lhs,
        const StaMatrix<S,COLS,NCOLSB>& rhs) {
    UInt i, j, k;
    StaMatrix<T,ROWS,NCOLSB> res;
    for (i = 0; i < ROWS; i++) {
        for (k = 0; k < COLS; k++) {
            for (j = 0; j < NCOLSB; j++) {
                res(i,j) += lhs(i,k) * rhs(k,j);
            }
        }
    }
    return res;
}

template<class T, UInt ROWS, UInt COLS>
DynMatrix<T> operator*(
        const StaMatrix<T,ROWS,COLS>& lhs,
        const DynMatrix<T>& rhs) {
    assert(COLS == rhs.nRows());
    UInt i, j, k;
    DynMatrix<T> res(ROWS, rhs.nCols());
    for (i = 0; i < ROWS; i++) {
        for (k = 0; k < COLS; k++) {
            for (j = 0; j < rhs.nCols(); j++) {
                res(i,j) += lhs(i,k) * rhs(k,j);
            }
        }
    }
    return res;
}
