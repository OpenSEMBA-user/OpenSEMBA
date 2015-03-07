#include "StaMatrix.h"

template <class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>::StaMatrix() {
   for (unsigned int i = 0; i < NROWS*NCOLS; i++) {
      _val[i] = T(0);
   }
}

template <class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>::StaMatrix(
 const StaMatrix<T,NROWS,NCOLS>& param) {
   unsigned int nRnC = NROWS * NCOLS;
   for (unsigned int i = 0; i < nRnC; i++) {
      _val[i] = (T) param._val[i];
   }
}

template <class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>::~StaMatrix() {
}
// =-=-=-=-=-=-=-= General methods =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
template <class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator=(
 const StaMatrix<T,NROWS,NCOLS>& param) {
   if (this == &param)
      return *this;
   for (unsigned int i = 0; i < NROWS*NCOLS; i++)
      _val[i] = param._val[i];
   return *this;
}

template <class T, unsigned int NROWS, unsigned int NCOLS>
inline T
StaMatrix<T,NROWS,NCOLS>::val(const unsigned int i) const {
   return _val[i];
}

template <class T, unsigned int NROWS, unsigned int NCOLS>
inline T&
StaMatrix<T,NROWS,NCOLS>::val(const unsigned int i) {
   return _val[i];
}

template <class T, unsigned int NROWS, unsigned int NCOLS>
inline T
StaMatrix<T,NROWS,NCOLS>::val(const unsigned int row, const unsigned int col) const {
   return _val[row * NCOLS + col];
}

template <class T, unsigned int NROWS, unsigned int NCOLS>
inline T&
StaMatrix<T,NROWS,NCOLS>::val(const unsigned int row, const unsigned int col) {
   return _val[row * NCOLS + col];
}

template <class T, unsigned int NROWS, unsigned int NCOLS>
inline const T*
StaMatrix<T,NROWS,NCOLS>::val() const {
   return _val;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
inline T
StaMatrix<T,NROWS,NCOLS>::operator()(
 const unsigned int row,
 const unsigned int col) const {
   return _val[row * NCOLS + col];
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
inline T&
StaMatrix<T,NROWS,NCOLS>::operator()(const unsigned int row, const unsigned int col) {
   return _val[row * NCOLS + col];
}

template <class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::assign(
 const StaMatrix<int,NROWS,NCOLS>& param) {
   for (unsigned int i = 0; i < NROWS; i++) {
      for (unsigned int j = 0; j < NCOLS; j++) {
         val(i,j) = param(i,j);
      }
   }
   return *this;
}

template <class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator=(const DynMatrix<double>& param) {
   assert(NROWS == param.nRows() && NCOLS == param.nCols());
   for (unsigned int i = 0; i < NROWS; i++)
      for (unsigned int j = 0; j < NCOLS; j++)
         val(i,j) = param(i,j);
   return *this;
}

template <class T, unsigned int NROWS, unsigned int NCOLS>
void
StaMatrix<T,NROWS,NCOLS>::printInfo() const {
   printInfo(NROWS, NCOLS);
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
inline StaMatrix<T, NROWS, NCOLS>
StaMatrix<T,NROWS,NCOLS>::operator +(
      StaMatrix<T, NROWS, NCOLS>& param) const {
   StaMatrix<T,NROWS,NCOLS> res;
   unsigned int n = NROWS*NCOLS;
   for (unsigned int i = 0; i < n; i++) {
      res._val[i] = _val[i] + param._val[i];
   }
   return res;
}

template <class T, unsigned int NROWS, unsigned int NCOLS>
void
StaMatrix<T,NROWS,NCOLS>::printInfo(unsigned int rows, unsigned int cols) const {
   unsigned int i, j;
   if (rows > NROWS || cols > NCOLS) {
      cerr << "ERROR: Unable print more rows or cols than available" << endl;
      cerr << "Terminating."                                         << endl;
      exit(MATRICES_ERROR);
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

template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NCOLS,NROWS>&
StaMatrix<T,NROWS,NCOLS>::invert() {
   this->internalInvert();
   return *this;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
inline unsigned int
StaMatrix<T,NROWS,NCOLS>::nCols() const {
   return NCOLS;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
inline unsigned int
StaMatrix<T,NROWS,NCOLS>::nRows() const {
   return NROWS;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NCOLS,NROWS>
StaMatrix<T,NROWS,NCOLS>::transpose() {
   StaMatrix<T,NCOLS,NROWS> res;
   for (unsigned int i = 0; i < NROWS; i++) {
      for (unsigned int j = 0; j < NCOLS; j++) {
         res(j,i) = val(i,j);
      }
   }
   return res;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator+=(const double param) {
   for (unsigned int i = 0; i < NROWS*NCOLS; i++)
      _val[i] += param;
   return *this;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator+=(
 const StaMatrix<T,NROWS,NCOLS>& param) {
   for (unsigned int i = 0; i < NROWS*NCOLS; i++) {
      _val[i] += param._val[i];
   }
   return *this;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator-=(
 const StaMatrix<T,NROWS,NCOLS>& param) {
   for (unsigned int i = 0; i < NROWS*NCOLS; i++) {
      _val[i] -= param._val[i];
   }
   return *this;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator*=(const T param) {
   for (unsigned int i = 0; i < NROWS*NCOLS; i++) {
      _val[i] *= param;
   }
   return *this;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator/=(const T param) {
   for (unsigned int i = 0; i < NROWS*NCOLS; i++) {
      _val[i] /= param;
   }
   return *this;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>
StaMatrix<T,NROWS,NCOLS>::operator*(T param) const {
   StaMatrix<T,NROWS,NCOLS> res;
   for (unsigned int i = 0; i < NROWS*NCOLS; i++)
      res._val[i] = _val[i] * param;
   return res;
}

template<class T, unsigned int NROWS, unsigned NCOLS>
bool
StaMatrix<T,NROWS,NCOLS>::operator==(
 const StaMatrix<T,NROWS,NCOLS>& param) const {
   for (unsigned int i = 0; i < NROWS*NCOLS; i++) {
      T diff = abs(_val[i] -param._val[i]);
      if (diff > numeric_limits<T>::epsilon() * 1e2) {
         return false;
      }
   }
   return true;
}

template<class T, unsigned int NROWS, unsigned NCOLS>
bool
StaMatrix<T,NROWS,NCOLS>::operator<(
 const StaMatrix<T,NROWS,NCOLS>& param) const {
   for (uint i = 0; i < (NROWS*NCOLS); i++) {
      if (val(i) < param.val(i)) {
         return true;
      }
      if (val(i) > param.val(i)) {
         return false;
      }
   }
   return false;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator=(const DynMatrix<int>& rhs) {
   assert(NROWS == rhs.nRows() && NCOLS == rhs.nCols());
   for (unsigned int i = 0; i < NROWS; i++)
      for (unsigned int j = 0; j < NCOLS; j++)
         val(i,j) = (T) rhs(i,j);
   return *this;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
void
StaMatrix<T,NROWS,NCOLS>::copy(vector<vector<T> > values) {
   assert(NROWS == values.size());
   for (unsigned int i = 0; i < NROWS; i++) {
      assert(values[i].size() == NCOLS);
      for (unsigned int j = 0; j < NCOLS; j++) {
         val(i,j) = values[i][j];
      }
   }
}

// =============== Not member functions =======================================
template<class T, class S, unsigned int NROWS, unsigned int NCOLS, unsigned int NCOLSB>
StaMatrix<T,NROWS,NCOLSB>
operator*(const StaMatrix<T,NROWS,NCOLS>& lhs,
          const StaMatrix<S,NCOLS,NCOLSB>& rhs) {
   unsigned int i, j, k;
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

template<class T, unsigned int NROWS, unsigned int NCOLS>
DynMatrix<T>
operator*(const StaMatrix<T,NROWS,NCOLS>& lhs, const DynMatrix<T>& rhs) {
   assert(NCOLS == rhs.nRows());
   unsigned int i, j, k;
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
