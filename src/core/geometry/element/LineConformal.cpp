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

#include "LineConformal.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

LineConformal::LineConformal() {
    checkCoordinates();
}

LineConformal::LineConformal(const Id id,
                             const CoordI3* v[2],
                             const Math::CVecR3& norm,
                             const Layer* lay,
                             const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat),
    LinI2(v) {

    checkCoordinates();

    norm_  = norm;
}

LineConformal::LineConformal(const CoordI3* v[2],
                             const Math::CVecR3& norm,
                             const Layer* lay,
                             const Model* mat)
:   Elem(lay, mat) {
    checkCoordinates();
    norm_  = norm;
}

LineConformal::LineConformal(const LineConformal& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs),
    LinI2(rhs) {
    norm_  = rhs.norm_;
}

LineConformal::~LineConformal() {

}

const CoordConf* LineConformal::getV(const std::size_t i) const {
	const Coordinate::Coordinate<Math::Int,3>* coord;
	coord = Line2<Math::Int>::getV(i);
    return coord->castTo<CoordConf>();
}

void LineConformal::setV(const std::size_t i, const CoordI3* coord) {
    LinI2::setV(i, coord);
    checkCoordinates();
}

ElemR* LineConformal::toUnstructured(const Coordinate::Group<CoordR3>& cG,
                                     const Grid3& grid) const {
    const CoordConf* vConf[2];
    vConf[0] = getV(0);
    vConf[1] = getV(1);

    Math::CVecR3 pos;
    const CoordR3* coord[2];
    CoordId coordId;
    for (std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        pos = grid.getPos(vConf[i]->pos());
        if (Math::Util::greater(vConf[i]->getLength(), 0.0, 1.0)) {
            Math::Int dir = vConf[i]->getDir();
            Math::Real length = vConf[i]->getLength();
            Math::CVecI3 cellAux = vConf[i]->pos();
            cellAux(dir)++;
            Math::CVecR3 posAux = grid.getPos(cellAux);
            Math::Real step = posAux(dir)-pos(dir);
            pos(dir) += step*length;
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

void LineConformal::printInfo() const {
    std::cout << "--- LineConformal info ---" << std::endl;
    std::cout << "Id: " << this->getId() << std::endl;
    for (std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        getV(i)->printInfo();
        std::cout << std::endl;
    }
}

void LineConformal::checkCoordinates() {
    for(std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        if (!this->getV(i)->is<CoordConf>()) {
            throw Error::Coord::NotConf(this->getV(i)->getId());
        }
    }
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
