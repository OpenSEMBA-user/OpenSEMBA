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
/* =================== Matrix procedures ========================
   + Mathematical matrices class.
   + Contains two implementations:
     - Dynamic (Dynamic matrices): Size can change in run time.
     - Static  (Static  matrices): Size set at compilation time.
   ============================================================== */

#ifndef SEMBA_MATH_MATRIX_MATRIX_H_
#define SEMBA_MATH_MATRIX_MATRIX_H_

#ifndef MATRICES_MODES
#   define MATRICES_MODES
#   define MATRICES_COL_MAJOR      0
#   define MATRICES_ROW_MAJOR      1
#endif

#include <vector>

#include "math/Types.h"
#include "math/vector/Cartesian.h"

namespace SEMBA {
namespace Math {
namespace Matrix {

template<class T>
class Matrix {
public:
    Matrix();
    virtual ~Matrix();
    virtual Size nCols() const = 0;
    virtual Size nRows() const = 0;
    virtual T operator()(const Size row, const Size col) const = 0;
    virtual T& operator()(const Size row, const Size col) = 0;
    virtual T& val(const Size ind) = 0;
    virtual T val(const Size row, const Size col) const = 0;
    virtual T& val(const Size row, const Size col) = 0;
    virtual T val(const Size ind) const = 0;
    virtual void copy(std::vector<std::vector<T> > values) = 0;

    void zeros();
    void eye();
    void cpLowerTri2UpperTri();
    void convertToArray(const Int mode, Real *res) const;

    std::vector<Vector::Cartesian<T,3> > convertToCartesianVector() const;
    std::vector<T> cpRowToVector(const UInt row) const;

    T getDeterminant3x3() const;
    T getDeterminant4x4() const;
    T maxVal() const;
    T maxValInCol(Size col) const;

    void sortRows();
    void sortRows(const Size iCol, const Size lCol);

    Size findFirstOcurrenceInColumns(const T* vec,
                                     const Size col,
                                     const Size vecSize) const;

    bool isSquare() const;
    bool isSymmetric() const;

    virtual void printInfo() const = 0;

protected:
    void internal_();
    bool isEQ_(const T* x1, const T* x2, const Size vS) const;
    bool isGEQ_(const T* x1, const T* x2, const Size vS) const;
    bool isLEQ_(const T* x1, const T* x2, const Size vS) const;

private:
    void QSRows_(Int p, Int r,
                 const Size iCol, const Size lCol);
    Int partitionRows_(Int p, Int r,
                       const Size iCol, const Size lCol);
    Size binarySearch_(const T* key,
                       const Size col,
                       const Size vecSize,
                       Size imin,
                       Size imax) const;
    void factorizeLU_(Size pivot[]);
    void invertFactorized_(const Size pivot[]);
};

namespace Error {

class Base : public std::exception {
public:
    Base() {}
    virtual ~Base() throw() {}
};

class Size : public Base {
public:
    Size() {}
    virtual ~Size() throw() {}

    const char* what() const throw() {
        return "Unable print more rows or cols than available";
    }
};

} /* namespace Error */
} /* namespace Matrix */
} /* namespace Math */
} /* namespace SEMBA */

#include "Matrix.hpp"

#endif
