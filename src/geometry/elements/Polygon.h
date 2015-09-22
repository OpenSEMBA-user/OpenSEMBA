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
 * Polygon.h
 *
 *  Created on: Jul 25, 2014
 *      Author: luis
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include <cstdlib>

using namespace std;

#include "base/error/ErrorNotImplemented.h"

#include "Surface.h"

class Polygon: public Surface<Real> {
public:
    Polygon();
	Polygon(const GroupCoordinates<CoordR3>& cG,
            const ElementId id,
            const vector<CoordinateId>& vId,
            const LayerId layerId = LayerId(0),
            const MatId   matId   = MatId(0));
    Polygon(const Polygon& rhs);
	virtual ~Polygon();
    
    DEFINE_CLONE(Polygon);
    
	UInt numberOfFaces      () const;
	UInt numberOfVertices   () const;
	UInt numberOfCoordinates() const;
    
	UInt numberOfSideVertices   (const UInt f) const;
	UInt numberOfSideCoordinates(const UInt f) const;
    
	const CoordR3* getV    (const UInt i) const;
	const CoordR3* getSideV(const UInt f,
                            const UInt i) const;
    
	const CoordR3* getVertex    (const UInt i) const;
	const CoordR3* getSideVertex(const UInt f,
                                 const UInt i) const;
    
	Real getArea() const;
	
	void setV(const UInt i, const CoordR3*);
    
    void printInfo() const;

private:
	vector<const CoordR3*> v_;
	// TODO void checkVertexCoplanarity() const;

};

#endif /* POLYGON_H_ */
