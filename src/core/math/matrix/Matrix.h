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
     - Dynamic (Dynamic matrices): std::size_t can change in run time.
     - Static  (Static  matrices): std::size_t set at compilation time.
   ============================================================== */

#pragma once

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
    virtual std::size_t nCols() const = 0;
    virtual std::size_t nRows() const = 0;
    virtual T operator() (const std::size_t row,
                          const std::size_t col) const = 0;
    virtual T& operator()(const std::size_t row,
                          const std::size_t col) = 0;

    std::size_t size() const;
    virtual T& val(const std::size_t ind) = 0;
    virtual T val(const std::size_t row, const std::size_t col) const = 0;
    virtual T& val(const std::size_t row, const std::size_t col) = 0;
    virtual T val(const std::size_t ind) const = 0;
    virtual void copy(std::vector<std::vector<T> > values) = 0;

    void zeros();
    void eye();
    void cpLowerTri2UpperTri();
    void convertToArray(const Int mode, Real *res) const;

    std::vector<Vector::Cartesian<T,3> > convertToCartesianVector() const;
    std::vector<T> cpRowToVector(const std::size_t row) const;
    void cpToCol(const size_t col, const std::vector<T>& vec);
    void cpToRow(const size_t row, const std::vector<T>& vec);

    T getDeterminant3x3() const;
    T getDeterminant4x4() const;
    T maxVal() const;
    T maxValInCol(std::size_t col) const;

    void sortRows();
    void sortRows(const std::size_t iCol, const std::size_t lCol);

    std::size_t findFirstOcurrenceInColumns(const T* vec,
                                     const std::size_t col,
                                     const std::size_t vecSize) const;

    bool isSquare() const;
    bool isSymmetric() const;
    bool factorizeQR_(Matrix<T>& Q, Matrix<T>& R);

    virtual void printInfo() const = 0;

protected:
    void internal_();
    bool isEQ_(const T* x1, const T* x2, const std::size_t vS) const;
    bool isGEQ_(const T* x1, const T* x2, const std::size_t vS) const;
    bool isLEQ_(const T* x1, const T* x2, const std::size_t vS) const;
    T belowDiagonalAbsSum_();

private:
    void QSRows_(std::size_t p, std::size_t r,
                 const std::size_t iCol, const std::size_t lCol);
    std::size_t partitionRows_(std::size_t p, std::size_t r,
                               const std::size_t iCol, const std::size_t lCol);
    std::size_t binarySearch_(const T* key,
                       const std::size_t col,
                       const std::size_t vecSize,
                       std::size_t imin,
                       std::size_t imax) const;
    void factorizeLU_(std::size_t pivot[]);
    void invertFactorized_(const std::size_t pivot[]);
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

