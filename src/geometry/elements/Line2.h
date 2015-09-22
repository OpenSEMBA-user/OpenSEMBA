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
 * Line2.h
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef LIN2_H_
#define LIN2_H_

#include "Line.h"

class Line2Base : public virtual LineBase {
public:
    Line2Base() {};
    virtual ~Line2Base() {};

    inline UInt numberOfCoordinates() const { return 2; }
};

template<class T>
class Line2 : public virtual Line<T>,
              public virtual Line2Base {
public:
    Line2();
    Line2(const GroupCoordinates<Coordinate<T,3> >&,
          const ElementId id,
          const CoordinateId vId[2],
          const LayerId layerId = LayerId(0),
          const MatId   matId   = MatId(0));
    Line2(const ElementId id,
          const Coordinate<T,3>* v[2],
          const LayerId layerId = LayerId(0),
          const MatId   matId   = MatId(0));
    Line2(GroupCoordinates<Coordinate<T,3> >&,
          const ElementId id,
          const Box<T,3>& box,
          const LayerId layerId = LayerId(0),
          const MatId   matId   = MatId(0));
    Line2(const GroupCoordinates<Coordinate<T,3> >&,
          const CoordinateId vId[2]);
    Line2(const Coordinate<T,3>* v[2]);
    Line2(GroupCoordinates<Coordinate<T,3> >&,
          const Box<T,3>& box);
    Line2(const Line2<T>& rhs);
    virtual ~Line2();
    
    DEFINE_CLONE(Line2<T>);

    bool isStructured(const Grid3&, const Real = Grid3::tolerance) const;

    const Coordinate<T,3>* getV    (const UInt i) const { return v_[i]; }
    const Coordinate<T,3>* getSideV(const UInt f,
                               const UInt i) const;

    const Coordinate<T,3>* getVertex    (const UInt i) const;
    const Coordinate<T,3>* getSideVertex(const UInt f,
                                         const UInt i) const;

    void setV(const UInt i, const Coordinate<T,3>* coord);

    ElemI* toStructured(const GroupCoordinates<CoordI3>&,
                        const Grid3&, const Real = Grid3::tolerance) const;
    ElemR* toUnstructured(const GroupCoordinates<CoordR3>&, const Grid3&) const;

    void printInfo() const;

private:
    static const SimplexLin<1> lin;
    const Coordinate<T,3>* v_[2];

    void setCoordinates(const GroupCoordinates<Coordinate<T,3> >&,
                        const CoordinateId vId[2]);
    void setCoordinates(const Coordinate<T,3>* v[2]);
    void setCoordinates(GroupCoordinates<Coordinate<T,3> >&,
                        const Box<T,3>& box);
};

typedef Line2Base   Lin2;
typedef Line2<Real> LinR2;
typedef Line2<Int > LinI2;

#endif /* LIN2_H_ */
