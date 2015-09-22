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
 * Quad4.h
 *
 *  Created on: Apr 10, 2014
 *      Author: luis
 */

#ifndef QUAD_H_
#define QUAD_H_

#include "Surface.h"

class QuadrilateralBase : public virtual SurfaceBase {
public:
    QuadrilateralBase() {}
    virtual ~QuadrilateralBase() {}

    UInt numberOfFaces   () const { return 4; }
    UInt numberOfVertices() const { return 4; }

    UInt numberOfSideVertices(const UInt f = 0) const { return 2; }
};

template<class T>
class Quadrilateral : public virtual Surface<T>,
                      public virtual QuadrilateralBase {
public:
	Quadrilateral();
    virtual ~Quadrilateral();
};

typedef QuadrilateralBase   Qua;
typedef Quadrilateral<Real> QuaR;
typedef Quadrilateral<Int > QuaI;

#endif /* QUAD4_H_ */
