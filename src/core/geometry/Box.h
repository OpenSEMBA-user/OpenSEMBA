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
/*
 * Box.h
 *
 *  Created on: Dec 18, 2014
 *      Author: luis
 */
#ifndef SRC_COMMON_GEOMETRY_BOX_H_
#define SRC_COMMON_GEOMETRY_BOX_H_

#include <limits>
#include <utility>

using namespace std;

#include "base/error/Error.h"
#include "math/CartesianVector.h"
#include "Grid.h"

template<Int D> class Grid;

template <class T, Int D>
class Box {
    typedef CartesianVector<T,D> CVecTD;
public:
    class ErrorNotPoint : public Error {
    public:
        ErrorNotPoint();
        virtual ~ErrorNotPoint() throw();
    };
    class ErrorNotLine : public Error {
    public:
        ErrorNotLine();
        virtual ~ErrorNotLine() throw();
    };
    class ErrorNotSurface : public Error {
    public:
        ErrorNotSurface();
        virtual ~ErrorNotSurface() throw();
    };
    class ErrorNotVolume : public Error {
    public:
        ErrorNotVolume();
        virtual ~ErrorNotVolume() throw();
    };

    Box();
    Box(const pair<CVecTD,CVecTD>& boundsMinMax);
    Box(const CVecTD& min, const CVecTD& max);
    virtual ~Box();

    Box<T,D>& operator= (const Box<T,D>& lBoxMin);

    bool operator>(const Box<T,D>& lBoxMin) const;
    bool operator<(const Box<T,D>& lBoxMax) const;
    bool operator>=(const Box<T,D>& lBoxMin) const;
    bool operator<=(const Box<T,D>& lBoxMax) const;
    bool operator==(const Box<T,D>& lBoxMin) const;

    void operator+=(const Box<T,D>& lBoxSource);
    void operator+(const Box<T,D>& lBoxSource);

    void operator<<(const CVecTD& p);
    void operator<<(const Box<T,D>& p);

    bool isPoint  () const { return (numberOfDifferentCoords() == 0); }
    bool isLine   () const { return (numberOfDifferentCoords() == 1); }
    bool isSurface() const { return (numberOfDifferentCoords() == 2); }
    bool isVolume () const { return (numberOfDifferentCoords() == 3); }

    CartesianAxis getDirection() const;
    CartesianAxis getNormal   () const;

    Box<T,D> intersect(const Box<T,D>& rhs) const;
    bool isIntersected (const Box<T,D>& lBox) const;
    bool isInnerPoint(const CVecTD& point) const;

    inline CVecTD getMin() const;
    inline CVecTD getMax() const;
    inline CVecTD getLength() const;

    vector<CVecTD> getPos() const;
    vector<CVecTD> getPosOfBound(CartesianAxis d, CartesianBound p) const;
    Box<T,D> getBoundAsBox(CartesianAxis d, CartesianBound p) const;
    CVecTD getBound(CartesianBound p) const;
    vector<Box<T,D>> chop(
            const CartesianVector<T,D> step = CartesianVector<T,D>(1,1,1)) const;
    vector<Box<T,D>> chop(
            const Grid<D>& grid) const;

    void set(const pair<CVecTD,CVecTD>& boundsMinMax);
    Box<T,D>& setInfinity();
    void scale(const Real factor);

    void printInfo() const;
private:
    CVecTD min_, max_;

    void setDefaultValues() {
        min_.setPlusInfty();
        max_.setMinusInfty();
    };

    UInt numberOfDifferentCoords() const;
};

template <class T, Int D>
std::ostream& operator<<(ostream& os, const Box<T,D>& rhs) {
   return os << "Min: " << rhs.getMin() << ", Max: " << rhs.getMax();
}

typedef Box<Real,3> BoxR3;
typedef Box<Int ,2> BoxI2;
typedef Box<Int, 3> BoxI3;

#include "Box.hpp"


#endif /* SRC_COMMON_GEOMETRY_BOX_H_ */
