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
   Dynamic(Size rows, Size cols);
   Dynamic(Size rows, Size cols, T** values);
   virtual ~Dynamic();
   Size nCols() const;
   Size nRows() const;
   T operator()(const Size row, const Size col) const;
   T& operator()(const Size row, const Size col);
   T val(const Size ind) const;
   T& val(const Size ind);
   T val(const Size row, const Size col) const;
   T& val(const Size row, const Size col);
   const T* valPtr(const Size row, const Size col) const;
   // ----------- assignment, copy -------------------------------------------
   Dynamic<T>& operator=(const Dynamic<T> &param);
   Dynamic<T>& operator=(const std::vector<T> &param);
   Dynamic<T>& operator=(const T &param);
   void copy(std::vector<std::vector<T> > values);
   void copy(const Dynamic<Real> &param);
   Dynamic<T> copy(const Size fRow,
                   const Size lRow,
                   const Size fCol,
                   const Size lCol) const;
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
   Dynamic<T> sub(std::pair<Size,Size>& rows, std::pair<Size,Size>& cols) const;
   bool operator==(const Dynamic<T>& param) const;
   bool operator<(const Dynamic<T>& param) const;
   Dynamic<T> convolute(const Dynamic<T>& param) const;
   Dynamic<T> invert();
   Dynamic<T>& transpose();
   Dynamic<T> reshape(Size rows, Size cols);
   Dynamic<T> reshape(Size rows, Size cols, Int order);
   Dynamic<T> eliminateColumns(Size first, Size last) const;
   Dynamic<T>& sortAndRemoveRepeatedRows_omp();
   void sortAndRemoveRepeatedRows();
   void removeRepeatedSortedRows();
   void sortRows_omp(const Size iCol, const Size lCol);
   void mergeSortedRows(const Dynamic<T>& a,
                        const Size iCol,
                        const Size lCol);
   void printInfo() const;
   void printInfo(Size, Size) const;
   void printInfo(Size firstRow,
                  Size lastRow,
                  Size firstCol,
                  Size lastCol) const;
private:
   T* _val;
   Size _nRows, _nCols;

   void resizeVal(const Size rows, const Size cols);
};

} /* namespace Matrix */
} /* namespace Math */
} /* namespace SEMBA */

#include "Dynamic.hpp"

#endif
