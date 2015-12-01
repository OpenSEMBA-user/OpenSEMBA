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

#ifndef SEMBA_GEOMETRY_ELEMENT_POLYGON_H_
#define SEMBA_GEOMETRY_ELEMENT_POLYGON_H_

#include "Surface.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Polygon: public Surface<Math::Real> {
public:
    Polygon();
    Polygon(const Id id,
            const std::vector<const CoordR3*>& v,
            const Layer* lay = NULL,
            const Model* mat = NULL);
    Polygon(const Polygon& rhs);
    virtual ~Polygon();
    
    SEMBA_CLASS_DEFINE_CLONE(Polygon);
    
    Size numberOfFaces      () const;
    Size numberOfVertices   () const;
    Size numberOfCoordinates() const;

    Size numberOfSideVertices   (const Size f) const;
    Size numberOfSideCoordinates(const Size f) const;

    const CoordR3* getV    (const Size i) const;
    const CoordR3* getSideV(const Size f,
                            const Size i) const;

    const CoordR3* getVertex    (const Size i) const;
    const CoordR3* getSideVertex(const Size f,
                                 const Size i) const;

    Math::Real getArea() const;

    void setV(const Size i, const CoordR3*);
    
    void printInfo() const;

private:
    std::vector<const CoordR3*> v_;
    // TODO void checkVertexCoplanarity() const;

};

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_ELEMENT_POLYGON_H_ */
