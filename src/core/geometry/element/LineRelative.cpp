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

#include "LineRelative.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

LineRelative::LineRelative() {

}

LineRelative::LineRelative(const Id id,
                           const CoordI3* v[2],
                           const Layer* lay,
                           const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat),
    LinI2(v) {

}

LineRelative::LineRelative(const CoordI3* v[2],
                           const Layer* lay,
                           const Model* mat)
:   Elem(lay, mat) {

}

LineRelative::LineRelative(const LineRelative& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs),
    LinI2(rhs) {

}

LineRelative::~LineRelative() {

}

ElemR* LineRelative::toUnstructured(const Coordinate::Group<CoordR3>& cG,
                                    const Grid3& grid) const {
    const CoordI3* v[2];
    v[0] = getV(0);
    v[1] = getV(1);
    Math::CVecR3 pos;
    const CoordR3* coord[2];
    CoordId coordId;
    for (std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        pos = grid.getPos(v[i]->pos());
        if (v[i]->is<CoordRel>()) {
            const CoordRel* vRel = v[i]->castTo<CoordRel>();
            for (std::size_t d = 0; d < 3; d++) {
                Math::Real length = vRel->rel()(d);
                Math::Int cellDir = vRel->pos()(d)+1;
                Math::Real posAux = grid.getPos(d, cellDir);
                Math::Real step = posAux - pos(d);
                pos(d) += step * length;
            }
        }
        coordId = this->getV(i)->getId();
        if (!cG.existId(coordId)) {
            throw Error::Coord::NotFound(coordId);
        }
        coord[i] = cG.getId(coordId);
        if (coord[i]->pos() != pos) {
            throw Error::Coord::NotCoincident(coordId);
        }
    }
    return new LinR2(this->getId(),
                     coord,
                     this->getLayer(),
                     this->getModel());
}

void LineRelative::printInfo() const {
    std::cout << "--- LineRelative info ---" << std::endl;
    std::cout << "Id: " << this->getId() << std::endl;
    for (std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        getV(i)->printInfo();
        std::cout << std::endl;
    }
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
