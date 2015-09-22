#include "DynMatrix.h"

template <class T>
DynMatrix<T>::DynMatrix() {
   _val = NULL;
   _nRows = 0;
   _nCols = 0;
   for (UInt i = 0; i < _nRows*_nCols; i++) {
      _val[i] = (T) 0.0;
   }
}

template <class T>
DynMatrix<T>::DynMatrix(const DynMatrix<T>& param) {
   _nRows = param._nRows;
   _nCols = param._nCols;
   UInt nRnC = _nRows * _nCols;
   _val = new T[nRnC];
   for (UInt i = 0; i < nRnC; i++) {
      _val[i] = (T) param._val[i];
   }
}

template <class T>
DynMatrix<T>::DynMatrix(UInt rows, UInt cols) {
   _nRows = rows;
   _nCols = cols;
   _val = new T[_nRows * _nCols];
   for (UInt i = 0; i < _nRows*_nCols; i++) {
      _val[i] = (T) 0.0;
   }
}

template <class T>
DynMatrix<T>::DynMatrix(UInt rows, UInt cols, T** values) {
   _nRows = rows;
   _nCols = cols;
   assert(values != NULL);
   for (UInt i = 0; i < _nRows; i++) {
      for (UInt j = 0; j < _nCols; j++) {
         val(i,j) = values[i][j];
      }
   }
}

template <class T>
DynMatrix<T>::~DynMatrix<T>() {
   if (_val != NULL) {
      delete [] _val;
   }
}

template <class T>
DynMatrix<T>&
DynMatrix<T>::operator=(const DynMatrix<T>& param) {
   if (this == &param) {
      return *this;
   }
   _nRows = param._nRows;
   _nCols = param._nCols;
   resizeVal(_nRows, _nCols);
   UInt nRnC = _nRows * _nCols;
   for (UInt i = 0; i < nRnC; i++) {
      _val[i] = param._val[i];
   }
   return *this;
}

template<class T>
inline UInt
DynMatrix<T>::nCols() const {
   return _nCols;
}

template<class T>
inline UInt
DynMatrix<T>::nRows() const {
   return _nRows;
}

template<class T>
inline DynMatrix<T>
DynMatrix<T>::sub(pair<Int, Int>& rows, pair<Int, Int>& cols) const {
   Int resRows = rows.second-rows.first+1;
   Int resCols = cols.second-cols.first+1;
   DynMatrix<T> res(resRows, resCols);
   for (Int i = rows.first; i <= rows.second; i++) {
      for (Int j = cols.first; j <= cols.second; j++) {
         res(i - rows.first,j - cols.first) = val(i,j);
      }
   }
   return res;
}

template <class T>
inline void
DynMatrix<T>::resizeVal(const UInt rows, const UInt cols) {
   _nRows = rows;
   _nCols = cols;
   if (_val != NULL) {
      delete [] _val;
   }
   _val = new T[_nRows*_nCols];
}

template<class T>
void
DynMatrix<T>::copy(vector<vector<T> > values) {
   UInt rows = values.size();
   UInt cols;
   if (rows > 0) {
      cols = values[0].size();
   } else {
      cols = 0;
   }
   resizeVal(rows, cols);
   for (UInt i = 0; i < _nRows; i++) {
      for (UInt j = 0; j < _nCols; j++) {
         val(i,j) = values[i][j];
      }
   }
}

template <class T>
DynMatrix<T>&
DynMatrix<T>::operator=(const vector<T>& param) {
   resizeVal(param.size(), 1);
   for (UInt i = 0; i < _nRows; i++) {
      val(i,0) = param[i];
   }
   return *this;
}

template <class T>
void
DynMatrix<T>::copy(const DynMatrix<Real>& param) {
   _nRows = param.nRows();
   _nCols = param.nCols();
   resizeVal(nRows(), nCols());
   for (UInt r = 0; r < _nRows; r++) {
      for (UInt c = 0; c < _nCols; c++) {
         val(r,c) = param(r,c);
      }
   }
}

template<class T>
DynMatrix<T>
DynMatrix<T>::copy(
 const UInt fRow,
 const UInt lRow,
 const UInt fCol,
 const UInt lCol) const {
   UInt rS = lRow - fRow;
   UInt cS = lCol - fCol;
   DynMatrix<T> res(rS, cS);
   for (UInt i = 0; i < rS; i++) {
      for (UInt j = 0; j < cS; j++) {
         res(i,j) = val(i+fRow, j+fCol);
      }
   }
   return res;
}

template<class T>
DynMatrix<T>&
DynMatrix<T>::sortAndRemoveRepeatedRows_omp() {
   sortRows_omp(0, nCols());
   removeRepeatedSortedRows();
   return *this;
}

template<class T>
void
DynMatrix<T>::sortAndRemoveRepeatedRows() {
   this->sortRows(0, nCols());
   removeRepeatedSortedRows();
}

template<class T>
void
DynMatrix<T>::removeRepeatedSortedRows() {
   vector<vector<T> > aux;
   aux.reserve(nRows());
   UInt nonRepeated = 0;
   UInt nCompared;
   if (nRows() <= 1 ) {
      nCompared = 0;
   } else {
      nCompared = nRows() - 1;
   }
   for (UInt i = 0; i < nCompared; i++) {
      bool areEqual = true;
      for (UInt j = 0; j < nCols(); j++) {
         areEqual &= (val(i,j) == val(i+1,j));
      }
      if (!areEqual) {
         nonRepeated++;
         aux.push_back(this->cpRowToVector(i));
         if (i == nRows() - 2) {
            nonRepeated++;
            aux.push_back(this->cpRowToVector(i+1));
         }
      }
      if ((i == nRows() - 2) && areEqual) {
         nonRepeated++;
         aux.push_back(this->cpRowToVector(i+1));
      }
   }
   _nRows = nonRepeated;
   copy(aux);
}


template<class T>
void
DynMatrix<T>::sortRows_omp(
 const UInt iCol, const UInt lCol) {
#ifdef USE_OPENMP
   UInt nT = (UInt) omp_get_max_threads();
   if (_nRows < nT || nT == 1) {
      this->sortRows(iCol, lCol);
      return;
   }
   // Each thread copies and sorts its respective work chunk.
   DynMatrix<T> chunk[nT];
   UInt cSize = nRows() / nT;
   UInt tId;
   #pragma omp parallel private (tId) shared(chunk)
   {
      tId = omp_get_thread_num();
      UInt fRow = tId * cSize;
      UInt lRow = (tId + 1) * cSize;
      if (tId + 1 == nT) {
         lRow = nRows();
      }
      chunk[tId] = copy(fRow,lRow,0,nCols());
      chunk[tId].sortRows(iCol, lCol);
   }
   // Combines all the chunks.
   *this = chunk[0];
   for (UInt i = 1; i < nT; i++) {
      mergeSortedRows(chunk[i], iCol, lCol);
   }
#else
   this->sortRows(iCol, lCol);
#endif
}


template <class T>
DynMatrix<T>&
DynMatrix<T>::operator=(const T &param) {
   for (UInt i = 0; i < _nRows; i++)
      for (UInt j = 0; j < _nCols; j++)
         _val[i][j] = param;
   return *this;
}

template <class T>
inline T
DynMatrix<T>::operator() (const UInt row, const UInt col) const {
   assert(row < _nRows && col < _nCols);
   return _val[row * _nCols + col];
}

template <class T>
inline T&
DynMatrix<T>::operator()(const UInt row, const UInt col) {
   assert(col < _nCols);
   assert(row < _nRows);
   return _val[row * _nCols + col];
}

template <class T>
inline T
DynMatrix<T>::val(const UInt i) const {
   assert(i < _nRows * _nCols);
   return _val[i];
}

template <class T>
inline T&
DynMatrix<T>::val(const UInt i) {
   assert(i < _nRows * _nCols);
   return _val[i];
}

template <class T>
inline T
DynMatrix<T>::val(const UInt row, const UInt col) const {
   assert(col < _nCols);
   assert(row < _nRows);
   return _val[row * _nCols + col];
}

template <class T>
inline T&
DynMatrix<T>::val(const UInt row, const UInt col) {
   assert(col < _nCols);
   assert(row < _nRows);
   return _val[row * _nCols + col];
}

template <class T>
const T*
DynMatrix<T>::valPtr(const UInt row, const UInt col) const {
   assert(row < _nRows && col < _nCols);
   return &_val[row * _nCols + col];
}

template <class T>
DynMatrix<T>
DynMatrix<T>::operator*(DynMatrix<T> &param) const {
   DynMatrix<T> res(_nRows, param._nCols);
   UInt i, j, k;
   for (i = 0; i < _nRows; i++) {
      for (k = 0; k < _nCols; k++) {
         for (j = 0; j < param._nCols; j++) {
            res(i,j) += val(i,k) * param(k,j);
         }
      }
   }
   return res;
}

template <class T>
DynMatrix<T>
DynMatrix<T>::operator+(DynMatrix<T> &param) const {
   assert(_nRows == param._nRows && _nCols == param._nCols);
   DynMatrix<T> res(_nRows, _nCols);
   UInt n = _nRows * _nCols;
   for (UInt i = 0; i < n; i++) {
      res._val[i] = _val[i] + param._val[i];
   }
   return res;
}

template <class T>
DynMatrix<T>
DynMatrix<T>::operator*(const T param) const {
   DynMatrix<T> res(_nRows, _nCols);
   for (UInt i = 0; i < _nRows; i++) {
      for (UInt j = 0; j < _nCols; j++) {
         res(i,j) = val(i,j) * param;
      }
   }
   return res;
}

template<class T>
DynMatrix<T>
DynMatrix<T>::eliminateColumns(
 UInt first, UInt last) const {
   UInt nColsToEliminate = last - first + 1;
   DynMatrix<T> res(_nRows, _nCols - nColsToEliminate);
   for (UInt r = 0; r < _nRows; r++) {
      for (UInt c = 0; c < first; c++) {
         res(r,c) = val(r,c);
      }
   }
   for (UInt r = 0; r < _nRows; r++) {
      for (UInt c = last+1; c < _nCols; c++) {
         res(r,c-nColsToEliminate) = val(r,c);
      }
   }
   return res;
}

template <class T>
DynMatrix<T>
DynMatrix<T>::operator/(const T param) const {
   UInt i,j;
   DynMatrix<T> res(_nRows, _nCols);
   for (i = 0; i < _nRows; i++)
      for (j = 0; j < _nCols; j++)
         res._val[i][j] /= param;
   return res;
}

template <class T>
DynMatrix<T>&
DynMatrix<T>::operator+=(const T param) {
   UInt n = _nRows * _nCols;
   for (UInt i = 0; i < n; i++)
      _val[i] += param;
   return *this;
}

template <class T>
DynMatrix<T>&
DynMatrix<T>::operator+=(const DynMatrix<T>& param) {
   assert(_nRows == param._nRows && _nCols == param._nCols);
   UInt n = _nRows * _nCols;
   for (UInt i = 0; i < n; i++)
      _val[i] += param._val[i];
   return *this;
}

template <class T>
DynMatrix<T>&
DynMatrix<T>::operator*=(const T param) {
   for (UInt i = 0; i < _nRows; i++) {
      for (UInt j = 0; j < _nCols; j++) {
         val(i,j) *= param;
      }
   }
   return *this;
}

template <class T>
DynMatrix<T>&
DynMatrix<T>::operator/=(const T param) {
   assert(_nRows == param._nRows && _nCols == param._nCols);
   UInt i, j;
   for (i = 0; i < _nRows; i++) {
      for (j = 0; j < _nCols; j++) {
         _val[i][j] /= param;
      }
   }
   return *this;
}


template <class T>
bool
DynMatrix<T>::operator==(const DynMatrix<T>& param) const {
   bool res = true;
   if (_nRows == param._nRows && _nCols == param._nCols) {
      for (UInt i = 0; i < _nRows; i++) {
         for (UInt j = 0; j < _nCols; j++) {
            T diff = abs(this->operator()(i,j) - param(i,j));
            res &= diff < numeric_limits<T>::epsilon() * 1e2;
         }
      }
      return res;
   } else
      return false;
}

template <class T>
bool
DynMatrix<T>::operator<(const DynMatrix<T>& param) const {
   assert(_nRows == param._nRows && _nCols == param._nCols);
   for (UInt i = 0; i < _nRows; i++)
      for (UInt j = 0; j < _nCols; j++)
         if (_val[i][j] < param._val[i][j])
            return true;
   return false;
}

template <class T>
DynMatrix<T>
DynMatrix<T>::convolute(const DynMatrix<T> &param) const {
   // PURPOSE:
   // Returns convolution of this row matrix vector and param.
   assert(_nCols == 1 && param._nCols == 1);
   UInt i, j, min, max;
   DynMatrix<T> res(_nRows + param._nRows - 1, 1);
   min = _nRows;
   max = 1;
   for (j = 1; j <= _nRows + param._nRows - 1; j++) {
      if (j <= _nRows)
         min = j;
      if (j + 1 - param._nRows >= 1)
         max = j + 1 - param._nRows;
      for (i = max; i <= min; i++)
         res(j-1,0) += val(i-1,0) * param(j-i,0);
   }
   return res;
}

template <class T>
void
DynMatrix<T>::printInfo() const {
   printInfo(_nRows, _nCols);
}

template <class T>
void
DynMatrix<T>::printInfo(UInt rows, UInt cols) const {
   UInt i, j;
   if (rows > _nRows || cols > _nCols) {
       throw typename MathMatrix<T>::ErrorSize();
   }
   cout << "Dimensions: " << _nRows << "x" << _nCols << endl;
   cout << "Stored values: " << endl;
   for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++)
         cout << setprecision(16) << val(i,j) << " ";
      cout << endl;
   }
}

template <class T>
void
DynMatrix<T>::printInfo(
 UInt fr, UInt lr,
 UInt fc, UInt lc) const {
   cout << "Dimensions: " << _nRows << "x" << _nCols << endl;
   cout << "Stored values: " << endl;
   for (UInt i = fr; i <= lr; i++) {
      for (UInt j = fc; j < lc; j++)
         cout << _val[i][j] << " ";
      cout << endl;
   }
}

template <class T>
DynMatrix<T>
DynMatrix<T>::reshape(UInt rows, UInt cols) {
   DynMatrix<T> res(rows, cols);
   res = reshape(rows,cols,1);
   return res;
}

template <class T>
DynMatrix<T>
DynMatrix<T>::reshape(UInt rows, UInt cols, Int order) {
   assert(_nRows * _nCols == rows*cols);
   DynMatrix<T> res(rows, cols);
   UInt i, j, k;
   k = 0;
   switch (order) {
   case 1:
      // Performs ordering taking elements from one row at a time.
      for (i = 0; i < rows; i++)
         for (j = 0; j < cols; j++) {
            res._val[i][j] = _val[k / _nCols][k % _nCols];
            k++;
         }
      break;
   case 2:
      // Performs ordering taking elements from one column at a time.
      for (i = 0; i < rows; i++)
         for (j = 0; j < cols; j++) {
            res._val[i][j] = _val[k % _nRows][k / _nRows];
            k++;
         }
      break;
   }
   return res;
}

template<class T>
DynMatrix<T>
DynMatrix<T>::kron(DynMatrix<T>& rhs) const {
   DynMatrix<T> res(_nRows + rhs._nRows, _nCols + rhs._nCols);
   for (UInt i = 0; i < _nRows; i++) {
      for (UInt j = 0; j < rhs._nRows; j++) {
         for (UInt k = 0; k < _nCols; k++) {
            for (UInt l = 0; l < rhs._nCols; l++) {
               res(i*_nRows + j, k*_nCols + l) = val(i,k) * rhs(j,l);
            }
         }
      }
   }
   return res;
}

template <class T>
DynMatrix<T>
DynMatrix<T>::invert() {
//   this->internalInvert();
//   return *this;
   DynMatrix<T> res(*this);
   res. internalInvert();
   return DynMatrix<T>(res);
}

template <class T>
void
DynMatrix<T>::mergeSortedRows(
 const DynMatrix<T>& a,
 const UInt iCol,
 const UInt lCol) {
   assert(this != &a);
   assert(lCol >= iCol);
   DynMatrix<T> b;
   b = *this;
   assert(a.nCols() == b.nCols());
   UInt m = a.nRows();
   UInt n = b.nRows();
   _nRows = m + n;
   _nCols = a.nCols();
   resizeVal(a.nRows() + b.nRows(), a.nCols());
   UInt i = 0, j = 0, k = 0;
   UInt vS = lCol - iCol + 1;
   while (i < m && j < n) {
      const T *aVal = a.valPtr(i,iCol);
      const T *bVal = b.valPtr(j,iCol);
      bool leq = this->isLEQ(aVal, bVal, vS);
      if (leq) {
         for (UInt col = 0; col < _nCols; col++) {
            val(k,col) = a(i,col);
         }
         i++;
      } else {
         for (UInt col = 0; col < _nCols; col++) {
            val(k,col) = b(j,col);
         }
         j++;
      }
        k++;
   }
   if (i < m) {
      for (UInt p = i; p < m; p++,k++) {
         for (UInt col = 0; col < _nCols; col++) {
            val(k,col) = a(p,col);
         }
      }
   } else {
      for (UInt p = j; p < n; p++,k++) {
         for (UInt col = 0; col < _nCols; col++) {
            val(k,col) = b(p,col);
         }
      }
   }
}

template <class T>
DynMatrix<T>&
DynMatrix<T>::transpose() {
   DynMatrix<T> temp = *this;
   // Assigns new values to _nRows and _nCols.
   _nRows = temp._nCols;
   _nCols = temp._nRows;
   // Assigns values to transposed matrix.
   for (UInt i = 0; i < _nRows; i++) {
      for (UInt j = 0; j < _nCols; j++) {
         val(i,j) = temp(j,i);
      }
   }
   return *this;
}
