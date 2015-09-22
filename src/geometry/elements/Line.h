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
 * Line.h
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */

#ifndef LINE_H_
#define LINE_H_

#include "math/SimplexLin.h"

#include "Element.h"

using namespace std;

class LineBase : public virtual ElementBase {
public:
    LineBase() {}
    virtual ~LineBase() {}

    inline UInt numberOfFaces   () const { return 2; }
    inline UInt numberOfVertices() const { return 2; }

    inline UInt numberOfSideVertices   (const UInt f = 0) const { return 1; }
    inline UInt numberOfSideCoordinates(const UInt f = 0) const { return 1; }
};

template<class T>
class Line : public virtual Element<T>,
             public virtual LineBase {
public:
    Line();
    virtual ~Line();
};

typedef LineBase   Lin;
typedef Line<Real> LinR;
typedef Line<Int > LinI;

#endif /* LINE_H_ */
