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
 * Polyhedron.h
 *
 *  Created on: Jul 24, 2014
 *      Author: luis
 */

#ifndef POLYHEDRON_H_
#define POLYHEDRON_H_

#include "Volume.h"
#include "Polygon.h"

class Polyhedron : public Volume<Real> {
public:
    class ErrorNotClosed : public Error {
    public:
        ErrorNotClosed();
        ~ErrorNotClosed() throw ();
    };

    Polyhedron();
    Polyhedron(const ElementId id,
               const vector<const Polygon*>& faces,
               const LayerId layerId = LayerId(0),
               const MatId   matId   = MatId(0));
    Polyhedron(const Polyhedron& rhs);
    virtual ~Polyhedron();

    DEFINE_CLONE(Polyhedron);

    bool isCurvedFace(const UInt f) const;

    UInt numberOfFaces      () const;
    UInt numberOfVertices   () const;
    UInt numberOfCoordinates() const;

    UInt numberOfSideVertices   (const UInt f = 0) const;
    UInt numberOfSideCoordinates(const UInt f = 0) const;
    const CoordR3* getV(const UInt i) const;
    const CoordR3* getSideV(const UInt f, const UInt i) const;
    const CoordR3* getVertex(const UInt i) const;
    const CoordR3* getSideVertex(const UInt f, const UInt i) const;

    const Polygon* getFace(const UInt f) const;
    Real getAreaOfFace(const UInt f) const;
    Real getVolume() const;

    void printInfo() const;

private:
    vector<const CoordR3*> v_;
    vector<const Polygon*> face_;

    void addV(const CoordR3*);
    void checkClosedness() const;
};

#endif /* POLYHEDRON_H_ */
