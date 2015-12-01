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
#ifndef DYNMATRIX_H_
#define DYNMATRIX_H_

#include <utility>
using namespace std;

#include "MathMatrix.h"

template<class T>
class DynMatrix : public MathMatrix<T> {
public:
   DynMatrix();
   DynMatrix(const DynMatrix<T>& param);
   DynMatrix(UInt rows, UInt cols);
   DynMatrix(UInt rows, UInt cols, T** values);
   virtual ~DynMatrix();
   UInt nCols() const;
   UInt nRows() const;
   T operator()(const UInt row, const UInt col) const;
   T& operator()(const UInt row, const UInt col);
   T val(const UInt ind) const;
   T& val(const UInt ind);
   T val(const UInt row, const UInt col) const;
   T& val(const UInt row, const UInt col);
   const T* valPtr(const UInt row, const UInt col) const;
   // ----------- assignment, copy -------------------------------------------
   DynMatrix<T>& operator=(const DynMatrix<T> &param);
   DynMatrix<T>& operator=(const vector<T> &param);
   DynMatrix<T>& operator=(const T &param);
   void copy(vector<vector<T> > values);
   void copy(const DynMatrix<Real> &param);
   DynMatrix<T> copy(
           const UInt fRow,
           const UInt lRow,
           const UInt fCol,
           const UInt lCol) const;
   // ----------- operations -------------------------------------------------
   DynMatrix<T> operator+(DynMatrix<T> &param) const;
   DynMatrix<T>& operator+=(const T param);
   DynMatrix<T>& operator+=(const DynMatrix& param);
   DynMatrix<T>& operator*=(const T param);
   DynMatrix<T>& operator/=(const T param);
   DynMatrix<T> operator*(DynMatrix<T> &param) const;
   DynMatrix<T> operator*(const T param) const;
   DynMatrix<T> operator/(const T param) const;
   DynMatrix<T> kron(DynMatrix<T>& param) const;
   DynMatrix<T> sub(pair<Int,Int>& rows, pair<Int,Int>& cols) const;
   bool operator==(const DynMatrix<T>& param) const;
   bool operator<(const DynMatrix<T>& param) const;
   DynMatrix<T> convolute(const DynMatrix<T>& param) const;
   DynMatrix<T> invert();
   DynMatrix<T>& transpose();
   DynMatrix<T> reshape(UInt rows, UInt cols);
   DynMatrix<T> reshape(UInt rows, UInt cols, Int order);
   DynMatrix<T> eliminateColumns(UInt first, UInt last) const;
   DynMatrix<T>& sortAndRemoveRepeatedRows_omp();
   void sortAndRemoveRepeatedRows();
   void removeRepeatedSortedRows();
   void sortRows_omp(const UInt iCol, const UInt lCol);
   void mergeSortedRows(
     const DynMatrix<T>& a,
     const UInt iCol,
     const UInt lCol);
   void printInfo() const;
   void printInfo(UInt, UInt) const;
   void printInfo(
     UInt firstRow,
     UInt lastRow,
     UInt firstCol,
     UInt lastCol) const;
private:
   T* _val;
   UInt _nRows, _nCols;
   void resizeVal(const UInt rows, const UInt cols);
};

#include "DynMatrix.hpp"

#endif
