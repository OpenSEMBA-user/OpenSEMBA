// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#include "Dynamic.h"

#include <iomanip>

namespace SEMBA {
namespace Math {
namespace Matrix {

template <class T>
Dynamic<T>::Dynamic() {
   _val = NULL;
   _nRows = 0;
   _nCols = 0;
   for (Size i = 0; i < _nRows*_nCols; i++) {
      _val[i] = (T) 0.0;
   }
}

template <class T>
Dynamic<T>::Dynamic(const Dynamic<T>& param) {
   _nRows = param._nRows;
   _nCols = param._nCols;
   Size nRnC = _nRows * _nCols;
   _val = new T[nRnC];
   for (Size i = 0; i < nRnC; i++) {
      _val[i] = (T) param._val[i];
   }
}

template <class T>
Dynamic<T>::Dynamic(Size rows, Size cols) {
   _nRows = rows;
   _nCols = cols;
   _val = new T[_nRows * _nCols];
   for (Size i = 0; i < _nRows*_nCols; i++) {
      _val[i] = (T) 0.0;
   }
}

template <class T>
Dynamic<T>::Dynamic(Size rows, Size cols, T** values) {
   _nRows = rows;
   _nCols = cols;
   assert(values != NULL);
   for (Size i = 0; i < _nRows; i++) {
      for (Size j = 0; j < _nCols; j++) {
         val(i,j) = values[i][j];
      }
   }
}

template <class T>
Dynamic<T>::~Dynamic() {
   if (_val != NULL) {
      delete [] _val;
   }
}

template <class T>
Dynamic<T>& Dynamic<T>::operator=(const Dynamic<T>& param) {
   if (this == &param) {
      return *this;
   }
   _nRows = param._nRows;
   _nCols = param._nCols;
   resizeVal(_nRows, _nCols);
   Size nRnC = _nRows * _nCols;
   for (Size i = 0; i < nRnC; i++) {
      _val[i] = param._val[i];
   }
   return *this;
}

template<class T>
inline Size Dynamic<T>::nCols() const {
   return _nCols;
}

template<class T>
inline Size Dynamic<T>::nRows() const {
   return _nRows;
}

template<class T>
inline Dynamic<T> Dynamic<T>::sub(std::pair<Size, Size>& rows,
                                  std::pair<Size, Size>& cols) const {
   Size resRows = rows.second-rows.first+1;
   Size resCols = cols.second-cols.first+1;
   Dynamic<T> res(resRows, resCols);
   for (Size i = rows.first; i <= rows.second; i++) {
      for (Size j = cols.first; j <= cols.second; j++) {
         res(i - rows.first,j - cols.first) = val(i,j);
      }
   }
   return res;
}

template <class T>
inline void Dynamic<T>::resizeVal(const Size rows, const Size cols) {
   _nRows = rows;
   _nCols = cols;
   if (_val != NULL) {
      delete [] _val;
   }
   _val = new T[_nRows*_nCols];
}

template<class T>
void Dynamic<T>::copy(std::vector<std::vector<T> > values) {
   Size rows = values.size();
   Size cols;
   if (rows > 0) {
      cols = values[0].size();
   } else {
      cols = 0;
   }
   resizeVal(rows, cols);
   for (Size i = 0; i < _nRows; i++) {
      for (Size j = 0; j < _nCols; j++) {
         val(i,j) = values[i][j];
      }
   }
}

template <class T>
Dynamic<T>&
Dynamic<T>::operator=(const std::vector<T>& param) {
   resizeVal(param.size(), 1);
   for (Size i = 0; i < _nRows; i++) {
      val(i,0) = param[i];
   }
   return *this;
}

template <class T>
void Dynamic<T>::copy(const Dynamic<Real>& param) {
   _nRows = param.nRows();
   _nCols = param.nCols();
   resizeVal(nRows(), nCols());
   for (Size r = 0; r < _nRows; r++) {
      for (Size c = 0; c < _nCols; c++) {
         val(r,c) = param(r,c);
      }
   }
}

template<class T>
Dynamic<T>
Dynamic<T>::copy(
 const Size fRow,
 const Size lRow,
 const Size fCol,
 const Size lCol) const {
   Size rS = lRow - fRow;
   Size cS = lCol - fCol;
   Dynamic<T> res(rS, cS);
   for (Size i = 0; i < rS; i++) {
      for (Size j = 0; j < cS; j++) {
         res(i,j) = val(i+fRow, j+fCol);
      }
   }
   return res;
}

template<class T>
Dynamic<T>&
Dynamic<T>::sortAndRemoveRepeatedRows_omp() {
   sortRows_omp(0, nCols());
   removeRepeatedSortedRows();
   return *this;
}

template<class T>
void
Dynamic<T>::sortAndRemoveRepeatedRows() {
   this->sortRows(0, nCols());
   removeRepeatedSortedRows();
}

template<class T>
void
Dynamic<T>::removeRepeatedSortedRows() {
   std::vector<std::vector<T> > aux;
   aux.reserve(nRows());
   Size nonRepeated = 0;
   Size nCompared;
   if (nRows() <= 1 ) {
      nCompared = 0;
   } else {
      nCompared = nRows() - 1;
   }
   for (Size i = 0; i < nCompared; i++) {
      bool areEqual = true;
      for (Size j = 0; j < nCols(); j++) {
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
Dynamic<T>::sortRows_omp(
 const Size iCol, const Size lCol) {
#ifdef USE_OPENMP
   Size nT = (Size) omp_get_max_threads();
   if (_nRows < nT || nT == 1) {
      this->sortRows(iCol, lCol);
      return;
   }
   // Each thread copies and sorts its respective work chunk.
   Dynamic<T> chunk[nT];
   Size cSize = nRows() / nT;
   Size tId;
   #pragma omp parallel private (tId) shared(chunk)
   {
      tId = omp_get_thread_num();
      Size fRow = tId * cSize;
      Size lRow = (tId + 1) * cSize;
      if (tId + 1 == nT) {
         lRow = nRows();
      }
      chunk[tId] = copy(fRow,lRow,0,nCols());
      chunk[tId].sortRows(iCol, lCol);
   }
   // Combines all the chunks.
   *this = chunk[0];
   for (Size i = 1; i < nT; i++) {
      mergeSortedRows(chunk[i], iCol, lCol);
   }
#else
   this->sortRows(iCol, lCol);
#endif
}


template <class T>
Dynamic<T>& Dynamic<T>::operator=(const T &param) {
   for (Size i = 0; i < _nRows; i++)
      for (Size j = 0; j < _nCols; j++)
         _val[i][j] = param;
   return *this;
}

template <class T>
inline T Dynamic<T>::operator() (const Size row, const Size col) const {
   assert(row < _nRows && col < _nCols);
   return _val[row * _nCols + col];
}

template <class T>
inline T& Dynamic<T>::operator()(const Size row, const Size col) {
   assert(col < _nCols);
   assert(row < _nRows);
   return _val[row * _nCols + col];
}

template <class T>
inline T Dynamic<T>::val(const Size i) const {
   assert(i < _nRows * _nCols);
   return _val[i];
}

template <class T>
inline T& Dynamic<T>::val(const Size i) {
   assert(i < _nRows * _nCols);
   return _val[i];
}

template <class T>
inline T Dynamic<T>::val(const Size row, const Size col) const {
   assert(col < _nCols);
   assert(row < _nRows);
   return _val[row * _nCols + col];
}

template <class T>
inline T& Dynamic<T>::val(const Size row, const Size col) {
   assert(col < _nCols);
   assert(row < _nRows);
   return _val[row * _nCols + col];
}

template <class T>
const T* Dynamic<T>::valPtr(const Size row, const Size col) const {
   assert(row < _nRows && col < _nCols);
   return &_val[row * _nCols + col];
}

template <class T>
Dynamic<T> Dynamic<T>::operator*(Dynamic<T> &param) const {
   Dynamic<T> res(_nRows, param._nCols);
   Size i, j, k;
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
Dynamic<T> Dynamic<T>::operator+(Dynamic<T> &param) const {
   assert(_nRows == param._nRows && _nCols == param._nCols);
   Dynamic<T> res(_nRows, _nCols);
   Size n = _nRows * _nCols;
   for (Size i = 0; i < n; i++) {
      res._val[i] = _val[i] + param._val[i];
   }
   return res;
}

template <class T>
Dynamic<T> Dynamic<T>::operator*(const T param) const {
   Dynamic<T> res(_nRows, _nCols);
   for (Size i = 0; i < _nRows; i++) {
      for (Size j = 0; j < _nCols; j++) {
         res(i,j) = val(i,j) * param;
      }
   }
   return res;
}

template<class T>
Dynamic<T> Dynamic<T>::eliminateColumns(Size first, Size last) const {
   Size nColsToEliminate = last - first + 1;
   Dynamic<T> res(_nRows, _nCols - nColsToEliminate);
   for (Size r = 0; r < _nRows; r++) {
      for (Size c = 0; c < first; c++) {
         res(r,c) = val(r,c);
      }
   }
   for (Size r = 0; r < _nRows; r++) {
      for (Size c = last+1; c < _nCols; c++) {
         res(r,c-nColsToEliminate) = val(r,c);
      }
   }
   return res;
}

template <class T>
Dynamic<T> Dynamic<T>::operator/(const T param) const {
   Size i,j;
   Dynamic<T> res(_nRows, _nCols);
   for (i = 0; i < _nRows; i++)
      for (j = 0; j < _nCols; j++)
         res._val[i][j] /= param;
   return res;
}

template <class T>
Dynamic<T>& Dynamic<T>::operator+=(const T param) {
   Size n = _nRows * _nCols;
   for (Size i = 0; i < n; i++)
      _val[i] += param;
   return *this;
}

template <class T>
Dynamic<T>& Dynamic<T>::operator+=(const Dynamic<T>& param) {
   assert(_nRows == param._nRows && _nCols == param._nCols);
   Size n = _nRows * _nCols;
   for (Size i = 0; i < n; i++)
      _val[i] += param._val[i];
   return *this;
}

template <class T>
Dynamic<T>& Dynamic<T>::operator*=(const T param) {
   for (Size i = 0; i < _nRows; i++) {
      for (Size j = 0; j < _nCols; j++) {
         val(i,j) *= param;
      }
   }
   return *this;
}

template <class T>
Dynamic<T>& Dynamic<T>::operator/=(const T param) {
   assert(_nRows == param._nRows && _nCols == param._nCols);
   Size i, j;
   for (i = 0; i < _nRows; i++) {
      for (j = 0; j < _nCols; j++) {
         _val[i][j] /= param;
      }
   }
   return *this;
}


template <class T>
bool Dynamic<T>::operator==(const Dynamic<T>& param) const {
   bool res = true;
   if (_nRows == param._nRows && _nCols == param._nCols) {
      for (Size i = 0; i < _nRows; i++) {
         for (Size j = 0; j < _nCols; j++) {
            T diff = abs(this->operator()(i,j) - param(i,j));
            res &= diff < std::numeric_limits<T>::epsilon() * 1e2;
         }
      }
      return res;
   } else
      return false;
}

template <class T>
bool Dynamic<T>::operator<(const Dynamic<T>& param) const {
   assert(_nRows == param._nRows && _nCols == param._nCols);
   for (Size i = 0; i < _nRows; i++)
      for (Size j = 0; j < _nCols; j++)
         if (_val[i][j] < param._val[i][j])
            return true;
   return false;
}

template <class T>
Dynamic<T> Dynamic<T>::convolute(const Dynamic<T> &param) const {
   // PURPOSE:
   // Returns convolution of this row matrix vector and param.
   assert(_nCols == 1 && param._nCols == 1);
   Size i, j, min, max;
   Dynamic<T> res(_nRows + param._nRows - 1, 1);
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
void Dynamic<T>::printInfo() const {
   printInfo(_nRows, _nCols);
}

template <class T>
void Dynamic<T>::printInfo(Size rows, Size cols) const {
   Size i, j;
   if (rows > _nRows || cols > _nCols) {
       throw typename Error::Size();
   }
   std::cout << "Dimensions: " << _nRows << "x" << _nCols << std::endl;
   std::cout << "Stored values: " << std::endl;
   for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++)
          std::cout << std::setprecision(16) << val(i,j) << " ";
      std::cout << std::endl;
   }
}

template <class T>
void Dynamic<T>::printInfo(Size fr, Size lr,
                           Size fc, Size lc) const {
   std::cout << "Dimensions: " << _nRows << "x" << _nCols << std::endl;
   std::cout << "Stored values: " << std::endl;
   for (Size i = fr; i <= lr; i++) {
      for (Size j = fc; j < lc; j++)
          std::cout << _val[i][j] << " ";
      std::cout << std::endl;
   }
}

template <class T>
Dynamic<T> Dynamic<T>::reshape(Size rows, Size cols) {
   Dynamic<T> res(rows, cols);
   res = reshape(rows,cols,1);
   return res;
}

template <class T>
Dynamic<T> Dynamic<T>::reshape(Size rows, Size cols, Int order) {
   assert(_nRows * _nCols == rows*cols);
   Dynamic<T> res(rows, cols);
   Size i, j, k;
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
Dynamic<T> Dynamic<T>::kron(Dynamic<T>& rhs) const {
   Dynamic<T> res(_nRows + rhs._nRows, _nCols + rhs._nCols);
   for (Size i = 0; i < _nRows; i++) {
      for (Size j = 0; j < rhs._nRows; j++) {
         for (Size k = 0; k < _nCols; k++) {
            for (Size l = 0; l < rhs._nCols; l++) {
               res(i*_nRows + j, k*_nCols + l) = val(i,k) * rhs(j,l);
            }
         }
      }
   }
   return res;
}

template <class T>
Dynamic<T> Dynamic<T>::invert() {
   Dynamic<T> res(*this);
   res.internal_();
   return Dynamic<T>(res);
}

template <class T>
void Dynamic<T>::mergeSortedRows(const Dynamic<T>& a,
                                 const Size iCol,
                                 const Size lCol) {
   assert(this != &a);
   assert(lCol >= iCol);
   Dynamic<T> b;
   b = *this;
   assert(a.nCols() == b.nCols());
   Size m = a.nRows();
   Size n = b.nRows();
   _nRows = m + n;
   _nCols = a.nCols();
   resizeVal(a.nRows() + b.nRows(), a.nCols());
   Size i = 0, j = 0, k = 0;
   Size vS = lCol - iCol + 1;
   while (i < m && j < n) {
      const T *aVal = a.valPtr(i,iCol);
      const T *bVal = b.valPtr(j,iCol);
      bool leq = this->isLEQ_(aVal, bVal, vS);
      if (leq) {
         for (Size col = 0; col < _nCols; col++) {
            val(k,col) = a(i,col);
         }
         i++;
      } else {
         for (Size col = 0; col < _nCols; col++) {
            val(k,col) = b(j,col);
         }
         j++;
      }
        k++;
   }
   if (i < m) {
      for (Size p = i; p < m; p++,k++) {
         for (Size col = 0; col < _nCols; col++) {
            val(k,col) = a(p,col);
         }
      }
   } else {
      for (Size p = j; p < n; p++,k++) {
         for (Size col = 0; col < _nCols; col++) {
            val(k,col) = b(p,col);
         }
      }
   }
}

template <class T>
Dynamic<T>& Dynamic<T>::transpose() {
   Dynamic<T> temp = *this;
   // Assigns new values to _nRows and _nCols.
   _nRows = temp._nCols;
   _nCols = temp._nRows;
   // Assigns values to transposed matrix.
   for (Size i = 0; i < _nRows; i++) {
      for (Size j = 0; j < _nCols; j++) {
         val(i,j) = temp(j,i);
      }
   }
   return *this;
}

} /* namespace Matrix */
} /* namespace Math */
} /* namespace SEMBA */
