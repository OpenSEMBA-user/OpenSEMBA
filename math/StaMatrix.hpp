#include "StaMatrix.h"

template <class T, UInt NROWS, UInt NCOLS>
StaMatrix<T,NROWS,NCOLS>::StaMatrix() {
   for (UInt i = 0; i < NROWS*NCOLS; i++) {
      _val[i] = T(0);
   }
}

template <class T, UInt NROWS, UInt NCOLS>
StaMatrix<T,NROWS,NCOLS>::StaMatrix(
 const StaMatrix<T,NROWS,NCOLS>& param) {
   UInt nRnC = NROWS * NCOLS;
   for (UInt i = 0; i < nRnC; i++) {
      _val[i] = (T) param._val[i];
   }
}

template <class T, UInt NROWS, UInt NCOLS>
StaMatrix<T,NROWS,NCOLS>::~StaMatrix() {
}
// =-=-=-=-=-=-=-= General methods =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
template <class T, UInt NROWS, UInt NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator=(
 const StaMatrix<T,NROWS,NCOLS>& param) {
   if (this == &param)
      return *this;
   for (UInt i = 0; i < NROWS*NCOLS; i++)
      _val[i] = param._val[i];
   return *this;
}

template <class T, UInt NROWS, UInt NCOLS>
inline T
StaMatrix<T,NROWS,NCOLS>::val(const UInt i) const {
   return _val[i];
}

template <class T, UInt NROWS, UInt NCOLS>
inline T&
StaMatrix<T,NROWS,NCOLS>::val(const UInt i) {
   return _val[i];
}

template <class T, UInt NROWS, UInt NCOLS>
inline T
StaMatrix<T,NROWS,NCOLS>::val(const UInt row, const UInt col) const {
   return _val[row * NCOLS + col];
}

template <class T, UInt NROWS, UInt NCOLS>
inline T&
StaMatrix<T,NROWS,NCOLS>::val(const UInt row, const UInt col) {
   return _val[row * NCOLS + col];
}

template <class T, UInt NROWS, UInt NCOLS>
inline const T*
StaMatrix<T,NROWS,NCOLS>::val() const {
   return _val;
}

template<class T, UInt NROWS, UInt NCOLS>
inline T
StaMatrix<T,NROWS,NCOLS>::operator()(
 const UInt row,
 const UInt col) const {
   return _val[row * NCOLS + col];
}

template<class T, UInt NROWS, UInt NCOLS>
inline T&
StaMatrix<T,NROWS,NCOLS>::operator()(const UInt row, const UInt col) {
   return _val[row * NCOLS + col];
}

template <class T, UInt NROWS, UInt NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::assign(
 const StaMatrix<Int,NROWS,NCOLS>& param) {
   for (UInt i = 0; i < NROWS; i++) {
      for (UInt j = 0; j < NCOLS; j++) {
         val(i,j) = param(i,j);
      }
   }
   return *this;
}

template <class T, UInt NROWS, UInt NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator=(const DynMatrix<Real>& param) {
   assert(NROWS == param.nRows() && NCOLS == param.nCols());
   for (UInt i = 0; i < NROWS; i++)
      for (UInt j = 0; j < NCOLS; j++)
         val(i,j) = param(i,j);
   return *this;
}

template <class T, UInt NROWS, UInt NCOLS>
void
StaMatrix<T,NROWS,NCOLS>::printInfo() const {
   printInfo(NROWS, NCOLS);
}

template<class T, UInt NROWS, UInt NCOLS>
inline StaMatrix<T, NROWS, NCOLS>
StaMatrix<T,NROWS,NCOLS>::operator +(
      StaMatrix<T, NROWS, NCOLS>& param) const {
   StaMatrix<T,NROWS,NCOLS> res;
   UInt n = NROWS*NCOLS;
   for (UInt i = 0; i < n; i++) {
      res._val[i] = _val[i] + param._val[i];
   }
   return res;
}

template <class T, UInt NROWS, UInt NCOLS>
void
StaMatrix<T,NROWS,NCOLS>::printInfo(UInt rows, UInt cols) const {
   UInt i, j;
   if (rows > NROWS || cols > NCOLS) {
       throw typename MathMatrix<T>::ErrorSize();
   }
   cout << "Matrix type: Double";
   cout << "Dimensions: " << NROWS << "x" << NCOLS << endl;
   cout << "Stored values: " << endl;
   for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++)
         cout << val(i,j) << " ";
      cout << endl;
   }
}

template<class T, UInt NROWS, UInt NCOLS>
StaMatrix<T,NCOLS,NROWS>&
StaMatrix<T,NROWS,NCOLS>::invert() {
   this->internalInvert();
   return *this;
}

template<class T, UInt NROWS, UInt NCOLS>
inline UInt
StaMatrix<T,NROWS,NCOLS>::nCols() const {
   return NCOLS;
}

template<class T, UInt NROWS, UInt NCOLS>
inline UInt
StaMatrix<T,NROWS,NCOLS>::nRows() const {
   return NROWS;
}

template<class T, UInt NROWS, UInt NCOLS>
StaMatrix<T,NCOLS,NROWS>
StaMatrix<T,NROWS,NCOLS>::transpose() {
   StaMatrix<T,NCOLS,NROWS> res;
   for (UInt i = 0; i < NROWS; i++) {
      for (UInt j = 0; j < NCOLS; j++) {
         res(j,i) = val(i,j);
      }
   }
   return res;
}

template<class T, UInt NROWS, UInt NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator+=(const Real param) {
   for (UInt i = 0; i < NROWS*NCOLS; i++)
      _val[i] += param;
   return *this;
}

template<class T, UInt NROWS, UInt NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator+=(
 const StaMatrix<T,NROWS,NCOLS>& param) {
   for (UInt i = 0; i < NROWS*NCOLS; i++) {
      _val[i] += param._val[i];
   }
   return *this;
}

template<class T, UInt NROWS, UInt NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator-=(
 const StaMatrix<T,NROWS,NCOLS>& param) {
   for (UInt i = 0; i < NROWS*NCOLS; i++) {
      _val[i] -= param._val[i];
   }
   return *this;
}

template<class T, UInt NROWS, UInt NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator*=(const T param) {
   for (UInt i = 0; i < NROWS*NCOLS; i++) {
      _val[i] *= param;
   }
   return *this;
}

template<class T, UInt NROWS, UInt NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator/=(const T param) {
   for (UInt i = 0; i < NROWS*NCOLS; i++) {
      _val[i] /= param;
   }
   return *this;
}

template<class T, UInt NROWS, UInt NCOLS>
StaMatrix<T,NROWS,NCOLS>
StaMatrix<T,NROWS,NCOLS>::operator*(T param) const {
   StaMatrix<T,NROWS,NCOLS> res;
   for (UInt i = 0; i < NROWS*NCOLS; i++)
      res._val[i] = _val[i] * param;
   return res;
}

template<class T, UInt NROWS, UInt NCOLS>
bool
StaMatrix<T,NROWS,NCOLS>::operator==(
 const StaMatrix<T,NROWS,NCOLS>& param) const {
   for (UInt i = 0; i < NROWS*NCOLS; i++) {
      T diff = abs(_val[i] -param._val[i]);
      if (diff > numeric_limits<T>::epsilon() * 1e2) {
         return false;
      }
   }
   return true;
}

template<class T, UInt NROWS, UInt NCOLS>
bool
StaMatrix<T,NROWS,NCOLS>::operator<(
 const StaMatrix<T,NROWS,NCOLS>& param) const {
   for (UInt i = 0; i < (NROWS*NCOLS); i++) {
      if (val(i) < param.val(i)) {
         return true;
      }
      if (val(i) > param.val(i)) {
         return false;
      }
   }
   return false;
}

template<class T, UInt NROWS, UInt NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator=(const DynMatrix<Int>& rhs) {
   assert(NROWS == rhs.nRows() && NCOLS == rhs.nCols());
   for (UInt i = 0; i < NROWS; i++)
      for (UInt j = 0; j < NCOLS; j++)
         val(i,j) = (T) rhs(i,j);
   return *this;
}

template<class T, UInt NROWS, UInt NCOLS>
void
StaMatrix<T,NROWS,NCOLS>::copy(vector<vector<T> > values) {
   assert(NROWS == values.size());
   for (UInt i = 0; i < NROWS; i++) {
      assert(values[i].size() == NCOLS);
      for (UInt j = 0; j < NCOLS; j++) {
         val(i,j) = values[i][j];
      }
   }
}

// =============== Not member functions =======================================
template<class T, class S, UInt NROWS, UInt NCOLS, UInt NCOLSB>
StaMatrix<T,NROWS,NCOLSB>
operator*(const StaMatrix<T,NROWS,NCOLS>& lhs,
          const StaMatrix<S,NCOLS,NCOLSB>& rhs) {
   UInt i, j, k;
   StaMatrix<T,NROWS,NCOLSB> res;
   for (i = 0; i < NROWS; i++) {
      for (k = 0; k < NCOLS; k++) {
         for (j = 0; j < NCOLSB; j++) {
            res(i,j) += lhs(i,k) * rhs(k,j);
         }
      }
   }
   return res;
}

template<class T, UInt NROWS, UInt NCOLS>
DynMatrix<T>
operator*(const StaMatrix<T,NROWS,NCOLS>& lhs, const DynMatrix<T>& rhs) {
   assert(NCOLS == rhs.nRows());
   UInt i, j, k;
   DynMatrix<T> res(NROWS, rhs.nCols());
   for (i = 0; i < NROWS; i++) {
      for (k = 0; k < NCOLS; k++) {
         for (j = 0; j < rhs.nCols(); j++) {
            res(i,j) += lhs(i,k) * rhs(k,j);
         }
      }
   }
   return res;
}
