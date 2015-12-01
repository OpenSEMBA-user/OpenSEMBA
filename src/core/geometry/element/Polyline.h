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
 * Polyline.h
 *
 *  Created on: Apr 15, 2015
 *      Author: damarro
 */

#ifndef POLYLINE_H_
#define POLYLINE_H_

#include <vector>
using namespace std;

#include "Line.h"

class PolylineBase : public virtual LineBase {
public:
    PolylineBase() {};
    virtual ~PolylineBase() {};
};

template<class T>
class Polyline : public virtual Line<T>,
                 public virtual PolylineBase {
public:
    Polyline();
    Polyline(const ElementId id,
             const vector<const Coordinate<T,3>*>& v,
             const LayerId layerId = LayerId(0),
             const MatId   matId   = MatId(0));
    Polyline(const Polyline<T>& rhs);
    virtual ~Polyline();
    
    DEFINE_CLONE(Polyline<T>);

    inline UInt numberOfCoordinates() const { return v_.size(); }

    const Coordinate<T,3>* getV    (const UInt i) const { return v_[i]; }
    const Coordinate<T,3>* getSideV(const UInt f,
                                    const UInt i) const;

    const Coordinate<T,3>* getVertex    (const UInt i) const;
    const Coordinate<T,3>* getSideVertex(const UInt f,
                                         const UInt i) const;

    void setV(const UInt i, const Coordinate<T,3>* coord);

    void printInfo() const;

private:
    vector<const Coordinate<T,3>*> v_;
};

typedef PolylineBase   Polylin;
typedef Polyline<Real> PolylinR;
typedef Polyline<Int > PolylinI;

#endif /* POLYLINE_H_ */
