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
 * Coordinate.h
 *
 *  Created on: Aug 26, 2012
 *      Author: luis
 */

#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <cstdlib>
#include <sstream>
#include <type_traits>
#include <vector>

using namespace std;

#include "geometry/Grid.h"
#include "math/CartesianVector.h"
#include "base/class/ClassIdBase.h"

class CoordinateBase;

typedef Id<CoordinateBase> CoordinateId;

class CoordinateBase : public virtual ClassIdBase<CoordinateId> {
public:
    CoordinateBase();
    virtual ~CoordinateBase();

    virtual bool operator==(const CoordinateBase& rhs) const;
    virtual bool operator!=(const CoordinateBase& rhs) const;
};

template <class T, Int D>
class Coordinate : public virtual CoordinateBase,
                   public virtual CartesianVector<T,D> {
public:
    Coordinate();
    Coordinate(const CoordinateId id_, const CartesianVector<T,D>& pos);
    explicit Coordinate(const CartesianVector<T,D>& pos);
    Coordinate(const Coordinate& rhs);
    virtual ~Coordinate();

    Coordinate<T,D>* clone() const;

    Coordinate& operator=(const Coordinate& rhs);

    bool operator==(const CoordinateBase& rhs) const;
    bool operator!=(const CoordinateBase& rhs) const;

    virtual bool isStructured(const Grid<D>&,
                              const Real = Grid<D>::tolerance) const;

    CartesianVector<T,D>&       pos()       { return *this; }
    const CartesianVector<T,D>& pos() const { return *this; }

    virtual Coordinate<Int ,D>* toStructured  (const Grid<D>&) const;
    virtual Coordinate<Real,D>* toUnstructured(const Grid<D>&) const;

    void printInfo() const;
};

typedef CoordinateBase      Coord;
typedef Coordinate<Real, 3> CoordR3;
typedef Coordinate<Int, 3> CoordI3;

#endif /* COORDINATE_H_ */
