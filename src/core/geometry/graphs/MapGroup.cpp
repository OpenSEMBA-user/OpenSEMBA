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
 * MapGroup.cpp
 *
 *  Created on: Jan 23, 2015
 *      Author: luis
 */

#include "../graphs/MapGroup.h"

MapGroup::ErrorHangingNode::ErrorHangingNode()
:   Error("Map Group: A surface has a hanging node") {

}

MapGroup::ErrorHangingNode::~ErrorHangingNode() throw () {

}

MapGroup::MapGroup() {
}

MapGroup::~MapGroup() {
}

MapGroup::MapGroup(
        const GroupCoordinates<const Coord>& cG,
        const GroupElements   <const Elem> & eG) {
    // Removes elements with no physical model associated.
    ElemRGroup elem = eG;
    elem.removeMatId(MatId(0));
    // Generates graph.
    elem_ = GraphVertices<ElemR,CoordR3>(elem);
}

const Tetrahedron* MapGroup::getNeighbour(const ElementId id, const UInt face) const {
    return tet_.find(id)->second->getNeighl(face);
}

UInt MapGroup::getVolToF(const ElementId id, const UInt face) const {
    return tet_.find(id)->second->getVolToF(face);
}


Face MapGroup::getInnerFace(const ElementId id) const {
    map<ElementId,MapSurface*>::const_iterator surf = tri_.find(id);
    assert(surf != tri_.end());
    const Tetrahedron* vol = surf->second->getVol(0);
    const UInt face = surf->second->getVolToF(0);
    return pair<const Tetrahedron*, UInt>(vol, face);
}

Face MapGroup::getOuterFace(const ElementId id) const {
    const VolR* vol = tri_.find(id)->second->getVol(1);
    const UInt face = tri_.find(id)->second->getVolToF(1);
    return pair<const VolR*, UInt>(vol, face);
}

Face MapGroup::getNeighConnection(
        const Face& face) const {
    Face res;
    res.first = getNeighbour(face.first->getId(), face.second);
    res.second = getVolToF(face.first->getId(), face.second);
    return res;
}

bool MapGroup::isBoundary(const ElementId  id) const {
    return tri_.find(id)->second->isBoundary();
}

bool MapGroup::isDomainBoundary(const ElementId id, const UInt f) const {
    return (getNeighbour(id,f)->getId() == ElementId(id));
}

bool MapGroup::isDomainBoundary(Face boundary) const {
    return isDomainBoundary(boundary.first->getId(), boundary.second);
}
