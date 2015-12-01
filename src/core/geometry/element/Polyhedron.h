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

#ifndef SEMBA_GEOMETRY_ELEMENT_POLYHEDRON_H_
#define SEMBA_GEOMETRY_ELEMENT_POLYHEDRON_H_

#include "Volume.h"
#include "Polygon.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Polyhedron : public Volume<Math::Real> {
public:
    Polyhedron();
    Polyhedron(const Id id,
               const std::vector<const Polygon*>& faces,
               const Layer* lay = NULL,
               const Model* mat = NULL);
    Polyhedron(const Polyhedron& rhs);
    virtual ~Polyhedron();

    SEMBA_CLASS_DEFINE_CLONE(Polyhedron);

    bool isCurvedFace(const Size f) const;

    Size numberOfFaces      () const;
    Size numberOfVertices   () const;
    Size numberOfCoordinates() const;

    Size numberOfSideVertices   (const Size f = 0) const;
    Size numberOfSideCoordinates(const Size f = 0) const;
    const CoordR3* getV(const Size i) const;
    const CoordR3* getSideV(const Size f, const Size i) const;
    const CoordR3* getVertex(const Size i) const;
    const CoordR3* getSideVertex(const Size f, const Size i) const;

    const Polygon* getFace(const Size f) const;
    Math::Real getAreaOfFace(const Size f) const;
    Math::Real getVolume() const;

    void printInfo() const;

private:
    std::vector<const CoordR3*> v_;
    std::vector<const Polygon*> face_;

    void addV(const CoordR3*);
    void checkClosedness() const;
};

namespace Error {

class NotClosed : public Error {
public:
    NotClosed() {}
    ~NotClosed() throw () {}

    const char* what() const throw() { return "Polyhedron is not closed"; }
};

} /* namespace Error */
} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_ELEMENT_POLYHEDRON_H_ */
