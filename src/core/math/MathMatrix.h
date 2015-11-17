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
/* =============== MathMatrix procedures ==============================
   + Mathematical matrices class.
   + Contains two implementations:
     - DynMatrix (Dynamic matrices): Size can change in run time.
     - StaMatrix (Static matrices): Size set at compilation time.
==================================================================== */
#ifndef MATHMATRIX_H_
#define MATHMATRIX_H_

#ifndef MATRICES_MODES
#	define MATRICES_MODES
#	define MATRICES_COL_MAJOR	   0
#	define MATRICES_ROW_MAJOR      1
#endif

#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <omp.h>
using namespace std;

#include "base/error/Error.h"
#include "CartesianVector.h"

template<class T>
class MathMatrix {
public:
    class ErrorSize : public Error {
    public:
        ErrorSize() : Error("Unable print more rows or cols than available") {}
        virtual ~ErrorSize() throw() {}
    };

    MathMatrix();
    virtual ~MathMatrix();
    virtual UInt nCols() const = 0;
    virtual UInt nRows() const = 0;
    virtual T operator()(const UInt row, const UInt col) const = 0;
    virtual T& operator()(const UInt row, const UInt col) = 0;
    virtual T& val(const UInt ind) = 0;
    virtual T val(const UInt row, const UInt col) const = 0;
    virtual T& val(const UInt row, const UInt col) = 0;
    virtual T val(const UInt ind) const = 0;
    virtual void copy(vector<vector<T> > values) = 0;

    void zeros();
    void eye();
    void cpLowerTri2UpperTri();
    void convertToArray(const Int mode, Real *res) const;

    vector<CartesianVector<T,3> > convertToCartesianVector() const;
    vector<T> cpRowToVector(const UInt row) const;

    T getDeterminant3x3() const;
    T getDeterminant4x4() const;
    T maxVal() const;
    T maxValInCol(UInt col) const;

    void sortRows();
    void sortRows(const UInt iCol, const UInt lCol);

    UInt findFirstOcurrenceInColumns(
            const T* vec,
            const UInt col,
            const UInt vecSize) const;

    bool isSquare() const;
    bool isSymmetric() const;


    virtual void printInfo() const = 0;
protected:
    void invert_();
    bool isEQ_(const T* x1, const T* x2, const UInt vS) const;
    bool isGEQ_(const T* x1, const T* x2, const UInt vS) const;
    bool isLEQ_(const T* x1, const T* x2, const UInt vS) const;
private:
    void QSRows_(Int p, Int r,
            const UInt iCol, const UInt lCol);
    Int partitionRows_(Int p, Int r,
            const UInt iCol, const UInt lCol);
    UInt binarySearch_(
            const T* key,
            const UInt col,
            const UInt vecSize,
            UInt imin,
            UInt imax) const;
    void factorizeLU(Int pivot[]);
    void invertFactorized(const Int pivot[]);
};

#include "MathMatrix.hpp"

#endif
