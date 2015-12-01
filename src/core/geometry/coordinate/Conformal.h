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
 * CoordinateConformal.h
 *
 *  Created on: 21/3/2015
 *      Author: Daniel
 */

#ifndef SRC_COMMON_GEOMETRY_COORDINATES_COORDINATECONFORMAL_H_
#define SRC_COMMON_GEOMETRY_COORDINATES_COORDINATECONFORMAL_H_

#include "Coordinate.h"

class CoordinateConformal : public virtual Coordinate<Int,3> {
public:
    CoordinateConformal();
    CoordinateConformal(const CoordinateId id_,
                        const CVecI3& pos,
                        const CartesianAxis dir, const Real length);
    CoordinateConformal(const CartesianAxis dir, const Real length);
    CoordinateConformal(const CoordinateConformal& rhs);
    virtual ~CoordinateConformal();

    DEFINE_CLONE(CoordinateConformal);

    CoordinateConformal& operator=(const CoordinateConformal& rhs);

    bool operator==(const CoordinateBase& rhs) const;

    CartesianAxis getDir   () const { return dir_;    }
    Real          getLength() const { return length_; }

    CoordR3* toUnstructured(const Grid3&) const;

    void printInfo() const;

private:
    CartesianAxis dir_;
    Real          length_;
};

typedef CoordinateConformal CoordConf;

#endif /* SRC_COMMON_GEOMETRY_COORDINATES_COORDINATECONFORMAL_H_ */
