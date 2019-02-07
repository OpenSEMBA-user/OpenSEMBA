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

#include "Tetrahedron.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

Tetrahedron::Tetrahedron() {

}

Tetrahedron::~Tetrahedron() {

}

Triangle3* Tetrahedron::getTri3Face(const std::size_t f) const {
    const CoordR3* coord[3];
    for (std::size_t i = 0; i < 3; i++) {
        coord[i] = getSideVertex(f,i);
    }
    return new Triangle3(Id(0), coord, getLayer(), getModel());
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
