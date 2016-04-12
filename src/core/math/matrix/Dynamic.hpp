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
   for (std::size_t i = 0; i < _nRows*_nCols; i++) {
      _val[i] = (T) 0.0;
   }
}

template <class T>
Dynamic<T>::Dynamic(const Dynamic<T>& param) {
   _nRows = param._nRows;
   _nCols = param._nCols;
   std::size_t nRnC = _nRows * _nCols;
   _val = new T[nRnC];
   for (std::size_t i = 0; i < nRnC; i++) {
      _val[i] = (T) param._val[i];
   }
}

template <class T>
Dynamic<T>::Dynamic(std::size_t rows, std::size_t cols) {
   _nRows = rows;
   _nCols = cols;
   _val = new T[_nRows * _nCols];
   for (std::size_t i = 0; i < _nRows*_nCols; i++) {
      _val[i] = (T) 0.0;
   }
}

template <class T>
Dynamic<T>::Dynamic(std::size_t rows, std::size_t cols, T** values) {
   _nRows = rows;
   _nCols = cols;
   assert(values != NULL);
   for (std::size_t i = 0; i < _nRows; i++) {
      for (std::size_t j = 0; j < _nCols; j++) {
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
   std::size_t nRnC = _nRows * _nCols;
   for (std::size_t i = 0; i < nRnC; i++) {
      _val[i] = param._val[i];
   }
   return *this;
}

template<class T>
inline std::size_t Dynamic<T>::nCols() const {
   return _nCols;
}

template<class T>
inline std::size_t Dynamic<T>::nRows() const {
   return _nRows;
}

template<class T>
inline Dynamic<T> Dynamic<T>::sub(
        std::pair<std::size_t, std::size_t>& rows,
        std::pair<std::size_t, std::size_t>& cols) const {
   std::size_t resRows = rows.second-rows.first+1;
   std::size_t resCols = cols.second-cols.first+1;
   Dynamic<T> res(resRows, resCols);
   for (std::size_t i = rows.first; i <= rows.second; i++) {
      for (std::size_t j = cols.first; j <= cols.second; j++) {
         res(i - rows.first,j - cols.first) = val(i,j);
      }
   }
   return res;
}

template <class T>
inline void Dynamic<T>::resizeVal(const std::size_t rows,
                                  const std::size_t cols) {
   _nRows = rows;
   _nCols = cols;
   if (_val != NULL) {
      delete [] _val;
   }
   _val = new T[_nRows*_nCols];
}

template<class T>
void Dynamic<T>::copy(std::vector<std::vector<T> > values) {
   std::size_t rows = values.size();
   std::size_t cols;
   if (rows > 0) {
      cols = values[0].size();
   } else {
      cols = 0;
   }
   resizeVal(rows, cols);
   for (std::size_t i = 0; i < _nRows; i++) {
      for (std::size_t j = 0; j < _nCols; j++) {
         val(i,j) = values[i][j];
      }
   }
}

template <class T>
Dynamic<T>&
Dynamic<T>::operator=(const std::vector<T>& param) {
   resizeVal(param.size(), 1);
   for (std::size_t i = 0; i < _nRows; i++) {
      val(i,0) = param[i];
   }
   return *this;
}

template <class T>
void Dynamic<T>::copy(const Dynamic<Real>& param) {
   _nRows = param.nRows();
   _nCols = param.nCols();
   resizeVal(nRows(), nCols());
   for (std::size_t r = 0; r < _nRows; r++) {
      for (std::size_t c = 0; c < _nCols; c++) {
         val(r,c) = param(r,c);
      }
   }
}

template<class T>
Dynamic<T>
Dynamic<T>::copy(
 const std::size_t fRow,
 const std::size_t lRow,
 const std::size_t fCol,
 const std::size_t lCol) const {
   std::size_t rS = lRow - fRow;
   std::size_t cS = lCol - fCol;
   Dynamic<T> res(rS, cS);
   for (std::size_t i = 0; i < rS; i++) {
      for (std::size_t j = 0; j < cS; j++) {
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
   std::size_t nonRepeated = 0;
   std::size_t nCompared;
   if (nRows() <= 1 ) {
      nCompared = 0;
   } else {
      nCompared = nRows() - 1;
   }
   for (std::size_t i = 0; i < nCompared; i++) {
      bool areEqual = true;
      for (std::size_t j = 0; j < nCols(); j++) {
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
 const std::size_t iCol, const std::size_t lCol) {
#ifdef USE_OPENMP
   std::size_t nT = (std::size_t) omp_get_max_threads();
   if (_nRows < nT || nT == 1) {
      this->sortRows(iCol, lCol);
      return;
   }
   // Each thread copies and sorts its respective work chunk.
   Dynamic<T> chunk[nT];
   std::size_t cSize = nRows() / nT;
   std::size_t tId;
   #pragma omp parallel private (tId) shared(chunk)
   {
      tId = omp_get_thread_num();
      std::size_t fRow = tId * cSize;
      std::size_t lRow = (tId + 1) * cSize;
      if (tId + 1 == nT) {
         lRow = nRows();
      }
      chunk[tId] = copy(fRow,lRow,0,nCols());
      chunk[tId].sortRows(iCol, lCol);
   }
   // Combines all the chunks.
   *this = chunk[0];
   for (std::size_t i = 1; i < nT; i++) {
      mergeSortedRows(chunk[i], iCol, lCol);
   }
#else
   this->sortRows(iCol, lCol);
#endif
}


template <class T>
Dynamic<T>& Dynamic<T>::operator=(const T &param) {
   for (std::size_t i = 0; i < _nRows; i++)
      for (std::size_t j = 0; j < _nCols; j++)
         _val[i][j] = param;
   return *this;
}

template <class T>
inline T Dynamic<T>::operator() (const std::size_t row,
                                 const std::size_t col) const {
   assert(row < _nRows && col < _nCols);
   return _val[row * _nCols + col];
}

template <class T>
inline T& Dynamic<T>::operator()(const std::size_t row,
                                 const std::size_t col) {
   assert(col < _nCols);
   assert(row < _nRows);
   return _val[row * _nCols + col];
}

template <class T>
inline T Dynamic<T>::val(const std::size_t i) const {
   assert(i < _nRows * _nCols);
   return _val[i];
}

template <class T>
inline T& Dynamic<T>::val(const std::size_t i) {
   assert(i < _nRows * _nCols);
   return _val[i];
}

template <class T>
inline T Dynamic<T>::val(const std::size_t row, const std::size_t col) const {
   assert(col < _nCols);
   assert(row < _nRows);
   return _val[row * _nCols + col];
}

template <class T>
inline T& Dynamic<T>::val(const std::size_t row, const std::size_t col) {
   assert(col < _nCols);
   assert(row < _nRows);
   return _val[row * _nCols + col];
}

template <class T>
const T* Dynamic<T>::valPtr(const std::size_t row,
                            const std::size_t col) const {
   assert(row < _nRows && col < _nCols);
   return &_val[row * _nCols + col];
}

template <class T>
Dynamic<T> Dynamic<T>::operator*(Dynamic<T> &param) const {
   Dynamic<T> res(_nRows, param._nCols);
   std::size_t i, j, k;
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
   std::size_t n = _nRows * _nCols;
   for (std::size_t i = 0; i < n; i++) {
      res._val[i] = _val[i] + param._val[i];
   }
   return res;
}

template <class T>
Dynamic<T> Dynamic<T>::operator*(const T param) const {
   Dynamic<T> res(_nRows, _nCols);
   for (std::size_t i = 0; i < _nRows; i++) {
      for (std::size_t j = 0; j < _nCols; j++) {
         res(i,j) = val(i,j) * param;
      }
   }
   return res;
}

template<class T>
Dynamic<T> Dynamic<T>::eliminateColumns(std::size_t first,
                                        std::size_t last) const {
   std::size_t nColsToEliminate = last - first + 1;
   Dynamic<T> res(_nRows, _nCols - nColsToEliminate);
   for (std::size_t r = 0; r < _nRows; r++) {
      for (std::size_t c = 0; c < first; c++) {
         res(r,c) = val(r,c);
      }
   }
   for (std::size_t r = 0; r < _nRows; r++) {
      for (std::size_t c = last+1; c < _nCols; c++) {
         res(r,c-nColsToEliminate) = val(r,c);
      }
   }
   return res;
}

template <class T>
Dynamic<T> Dynamic<T>::operator/(const T param) const {
   std::size_t i,j;
   Dynamic<T> res(_nRows, _nCols);
   for (i = 0; i < _nRows; i++)
      for (j = 0; j < _nCols; j++)
         res._val[i][j] /= param;
   return res;
}

template <class T>
Dynamic<T>& Dynamic<T>::operator+=(const T param) {
   std::size_t n = _nRows * _nCols;
   for (std::size_t i = 0; i < n; i++)
      _val[i] += param;
   return *this;
}

template <class T>
Dynamic<T>& Dynamic<T>::operator+=(const Dynamic<T>& param) {
   assert(_nRows == param._nRows && _nCols == param._nCols);
   std::size_t n = _nRows * _nCols;
   for (std::size_t i = 0; i < n; i++)
      _val[i] += param._val[i];
   return *this;
}

template <class T>
Dynamic<T>& Dynamic<T>::operator*=(const T param) {
   for (std::size_t i = 0; i < _nRows; i++) {
      for (std::size_t j = 0; j < _nCols; j++) {
         val(i,j) *= param;
      }
   }
   return *this;
}

template <class T>
Dynamic<T>& Dynamic<T>::operator/=(const T param) {
   assert(_nRows == param._nRows && _nCols == param._nCols);
   std::size_t i, j;
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
      for (std::size_t i = 0; i < _nRows; i++) {
         for (std::size_t j = 0; j < _nCols; j++) {
            T diff = std::abs(this->operator()(i,j) - param(i,j));
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
   for (std::size_t i = 0; i < _nRows; i++)
      for (std::size_t j = 0; j < _nCols; j++)
         if (_val[i][j] < param._val[i][j])
            return true;
   return false;
}

template <class T>
Dynamic<T> Dynamic<T>::convolute(const Dynamic<T> &param) const {
   // PURPOSE:
   // Returns convolution of this row matrix vector and param.
   assert(_nCols == 1 && param._nCols == 1);
   std::size_t i, j, min, max;
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
void Dynamic<T>::printInfo(std::size_t rows, std::size_t cols) const {
   std::size_t i, j;
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
void Dynamic<T>::printInfo(std::size_t fr, std::size_t lr,
                           std::size_t fc, std::size_t lc) const {
   std::cout << "Dimensions: " << _nRows << "x" << _nCols << std::endl;
   std::cout << "Stored values: " << std::endl;
   for (std::size_t i = fr; i <= lr; i++) {
      for (std::size_t j = fc; j < lc; j++)
          std::cout << _val[i][j] << " ";
      std::cout << std::endl;
   }
}

template <class T>
Dynamic<T> Dynamic<T>::reshape(std::size_t rows, std::size_t cols) {
   Dynamic<T> res(rows, cols);
   res = reshape(rows,cols,1);
   return res;
}

template <class T>
Dynamic<T> Dynamic<T>::reshape(std::size_t rows, std::size_t cols, Int order) {
   assert(_nRows * _nCols == rows*cols);
   Dynamic<T> res(rows, cols);
   std::size_t i, j, k;
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
   for (std::size_t i = 0; i < _nRows; i++) {
      for (std::size_t j = 0; j < rhs._nRows; j++) {
         for (std::size_t k = 0; k < _nCols; k++) {
            for (std::size_t l = 0; l < rhs._nCols; l++) {
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
                                 const std::size_t iCol,
                                 const std::size_t lCol) {
   assert(this != &a);
   assert(lCol >= iCol);
   Dynamic<T> b;
   b = *this;
   assert(a.nCols() == b.nCols());
   std::size_t m = a.nRows();
   std::size_t n = b.nRows();
   _nRows = m + n;
   _nCols = a.nCols();
   resizeVal(a.nRows() + b.nRows(), a.nCols());
   std::size_t i = 0, j = 0, k = 0;
   std::size_t vS = lCol - iCol + 1;
   while (i < m && j < n) {
      const T *aVal = a.valPtr(i,iCol);
      const T *bVal = b.valPtr(j,iCol);
      bool leq = this->isLEQ_(aVal, bVal, vS);
      if (leq) {
         for (std::size_t col = 0; col < _nCols; col++) {
            val(k,col) = a(i,col);
         }
         i++;
      } else {
         for (std::size_t col = 0; col < _nCols; col++) {
            val(k,col) = b(j,col);
         }
         j++;
      }
        k++;
   }
   if (i < m) {
      for (std::size_t p = i; p < m; p++,k++) {
         for (std::size_t col = 0; col < _nCols; col++) {
            val(k,col) = a(p,col);
         }
      }
   } else {
      for (std::size_t p = j; p < n; p++,k++) {
         for (std::size_t col = 0; col < _nCols; col++) {
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
   for (std::size_t i = 0; i < _nRows; i++) {
      for (std::size_t j = 0; j < _nCols; j++) {
         val(i,j) = temp(j,i);
      }
   }
   return *this;
}

/**
 * Computes the upper-Hessenberg form of the matrix through
 * the elimination method. Adapted from pages 485-486 (509-510 in the pdf numeration)
 * of the Numerical Recipes in C:
 * http://www2.units.it/ipl/students_area/imm2/files/Numerical_Recipes.pdf
 * See also http://www.math.iit.edu/~fass/477577_Chapter_9.pdf for
 * the usual method
 * @return      Returns the upper-hessenberg form of the caller.
 */
template<class T>
Dynamic<T> Dynamic<T>::hessenbergForm(){
    #define SWAP(g,h) {y=(g);(g)=(h);(h)=y;}

    int m,j,i;
    float y,x;

    int n = _nCols;

    Dynamic<T> H = *this;

    for (m = 1; m < n-1; m++) {
        x = 0.0;
        i = m;

        // Find the pivot
        for (j = m; j < n; j++) {
            if (fabs(H.val(j, m-1)) > fabs(x)) {
                x=H.val(j, m-1);
                i=j;
            }
        }

        // Interchange rows and columns
        if (i != m) {
            for (j = m-1; j < n; j++) SWAP(H.val(i, j),H.val(m, j))
            for (j = 0; j < n; j++) SWAP(H.val(j, i),H.val(j, m))
        }

        // Carry out the elimination
        if (x) {
            for (i = m+1; i < n; i++) {
                if ((y=H.val(i, m-1)) != 0.0) {
                    y/=x;
                    H.val(i, m-1)=y;

                    for (j = m; j < n; j++)
                        H.val(i, j) -= y*H.val(m, j);
                    for (j = 0; j < n; j++)
                        H.val(j, m) += y*H.val(j, i);
                }
            }
        }
    }

    // Values set to 0.0
    for (size_t i = 0; i < _nRows; i++) {
        for (size_t j = 0; j < i; j++) {
            H.val(i,j) = 0.0;
        }
    }

    return H;
}


/**
 * Computes the eigen values of the caller, using no more than maxIter
 * iterations and trying to get an error of tol
 * @param  maxIter Maximum number of iterations in the QR iterative algorithm
 * @param  tol     Accepted error between in the eigenvalues <- TODO
 * @return         Returns a std vector filled with the eigenvalues in descending
 *                 order.
 */
template<class T>
std::vector<T> Dynamic<T>::computeEigenvalues_(int maxIter, float tol) {
    assert(nRows() == nCols());

    Dynamic<T> A = *this;

    Dynamic<T> Q(_nRows, _nCols);
    Dynamic<T> R(_nRows, _nCols);

    for (size_t i = 0; i < maxIter /*&& A.belowDiagonalAbsSum_() > tol*/; i++) {
        A.factorizeQR_(Q, R);
        A = R*Q;
    }

    std::vector<T> eigenvalues;

    for (size_t i = 0; i < _nCols; i++) {
        eigenvalues.push_back(A.val(i, i));
    }

    return eigenvalues;
}

} /* namespace Matrix */
} /* namespace Math */
} /* namespace SEMBA */

// a\[([^\[]*)\]\[([^\[]*)\]
