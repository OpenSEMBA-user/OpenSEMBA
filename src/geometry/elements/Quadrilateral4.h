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

#ifndef QUAD4_H_
#define QUAD4_H_

#include <geometry/elements/Quadrilateral.h>

class Quadrilateral4Base : public virtual SurfaceBase {
public:
    Quadrilateral4Base() {}
    virtual ~Quadrilateral4Base() {}

    UInt numberOfCoordinates() const { return 4; }

    UInt numberOfSideCoordinates(const UInt f = 0) const { return 2; }
};

template<class T>
class Quadrilateral4: public virtual Quadrilateral<T>,
                      public virtual Quadrilateral4Base {
public:
    Quadrilateral4();
	Quadrilateral4(const GroupCoordinates<Coordinate<T,3> >&,
          const ElementId id,
          const CoordinateId vId[4],
          const LayerId layerId = LayerId(0),
          const MatId   matId   = MatId(0));
	Quadrilateral4(GroupCoordinates<Coordinate<T,3> >&,
	      const ElementId id,
	      const Box<T,3>& box,
	      const LayerId layerId = LayerId(0),
	      const MatId   matId   = MatId(0));
    Quadrilateral4(const Quadrilateral4<T>& rhs);
	virtual ~Quadrilateral4();
    
    DEFINE_CLONE(Quadrilateral4<T>);

    bool isStructured(const Grid3&, const Real = Grid3::tolerance) const;

	const Coordinate<T,3>* getV    (const UInt i) const { return v_[i]; }
	const Coordinate<T,3>* getSideV(const UInt f,
	                                const UInt i) const;

	const Coordinate<T,3>* getVertex    (const UInt i) const;
	const Coordinate<T,3>* getSideVertex(const UInt f,
	                                     const UInt i) const;

    void setV(const UInt i, const Coordinate<T,3>*);

    ElemI* toStructured(const GroupCoordinates<CoordI3>&,
                        const Grid3&, const Real = Grid3::tolerance) const;
    ElemR* toUnstructured(const GroupCoordinates<CoordR3>&, const Grid3&) const;

	void printInfo() const;
	void check() const;

private:
	const Coordinate<T,3>* v_[4];
};

typedef Quadrilateral4Base   Qua4;
typedef Quadrilateral4<Real> QuaR4;
typedef Quadrilateral4<Int > QuaI4;

#endif /* QUAD4_H_ */
