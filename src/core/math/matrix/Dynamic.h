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

#ifndef SEMBA_MATH_MATRIX_DYNAMIC_H_
#define SEMBA_MATH_MATRIX_DYNAMIC_H_

#include "Matrix.h"

namespace SEMBA {
namespace Math {
namespace Matrix {

template<class T>
class Dynamic : public Matrix<T> {
public:
   Dynamic();
   Dynamic(const Dynamic<T>& param);
   Dynamic(std::size_t rows, std::size_t cols);
   Dynamic(std::size_t rows, std::size_t cols, T** values);
   virtual ~Dynamic();
   std::size_t nCols() const;
   std::size_t nRows() const;
   T operator()(const std::size_t row, const std::size_t col) const;
   T& operator()(const std::size_t row, const std::size_t col);
   T val(const std::size_t ind) const;
   T& val(const std::size_t ind);
   T val(const std::size_t row, const std::size_t col) const;
   T& val(const std::size_t row, const std::size_t col);
   const T* valPtr(const std::size_t row, const std::size_t col) const;
   // ----------- assignment, copy -------------------------------------------
   Dynamic<T>& operator=(const Dynamic<T> &param);
   Dynamic<T>& operator=(const std::vector<T> &param);
   Dynamic<T>& operator=(const T &param);
   void copy(std::vector<std::vector<T> > values);
   void copy(const Dynamic<Real> &param);
   Dynamic<T> copy(const std::size_t fRow,
                   const std::size_t lRow,
                   const std::size_t fCol,
                   const std::size_t lCol) const;
   // ----------- operations -------------------------------------------------
   Dynamic<T> operator+(Dynamic<T> &param) const;
   Dynamic<T>& operator+=(const T param);
   Dynamic<T>& operator+=(const Dynamic& param);
   Dynamic<T>& operator*=(const T param);
   Dynamic<T>& operator/=(const T param);
   Dynamic<T> operator*(Dynamic<T> &param) const;
   Dynamic<T> operator*(const T param) const;
   Dynamic<T> operator/(const T param) const;
   Dynamic<T> kron(Dynamic<T>& param) const;
   Dynamic<T> sub(std::pair<std::size_t,std::size_t>& rows,
                  std::pair<std::size_t,std::size_t>& cols) const;
   bool operator==(const Dynamic<T>& param) const;
   bool operator<(const Dynamic<T>& param) const;
   Dynamic<T> convolute(const Dynamic<T>& param) const;
   Dynamic<T> invert();
   Dynamic<T>& transpose();
   Dynamic<T> reshape(std::size_t rows, std::size_t cols);
   Dynamic<T> reshape(std::size_t rows, std::size_t cols, Int order);
   Dynamic<T> eliminateColumns(std::size_t first, std::size_t last) const;
   Dynamic<T>& sortAndRemoveRepeatedRows_omp();

   void sortAndRemoveRepeatedRows();
   void removeRepeatedSortedRows();
   void sortRows_omp(const std::size_t iCol, const std::size_t lCol);
   void mergeSortedRows(const Dynamic<T>& a,
                        const std::size_t iCol,
                        const std::size_t lCol);
   void printInfo() const;
   void printInfo(std::size_t, std::size_t) const;
   void printInfo(std::size_t firstRow,
                  std::size_t lastRow,
                  std::size_t firstCol,
                  std::size_t lastCol) const;

#ifdef EIGEN_SUPPORT
    std::vector<std::complex<Real>> eigenValues();
    Dynamic<std::complex<Real>> eigenVectors();
#endif

private:
   T* _val;
   std::size_t _nRows, _nCols;

   void resizeVal(const std::size_t rows, const std::size_t cols);
};

} /* namespace Matrix */
} /* namespace Math */
} /* namespace SEMBA */

#include "Dynamic.hpp"

#endif
