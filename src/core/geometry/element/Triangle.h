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

#ifndef SEMBA_GEOMETRY_ELEMENT_TRIANGLE_H_
#define SEMBA_GEOMETRY_ELEMENT_TRIANGLE_H_

#include "math/simplex/Triangle.h"

#include "Surface.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Triangle : public Surface<Math::Real> {
public:
    Triangle();
    virtual ~Triangle();

    std::size_t numberOfFaces   () const { return 3; }
    std::size_t numberOfVertices() const { return 3; }

    std::size_t numberOfSideVertices(const std::size_t f = 0) const {
        return 2;
    }
};

} /* namespace Element */

typedef Element::Triangle Tri;

} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_ELEMENT_TRIANGLE_H_ */
