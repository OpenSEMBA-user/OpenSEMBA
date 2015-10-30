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

#include "Connectivities.h"

Connectivities::Connectivities() {
}

Connectivities::~Connectivities() {
}

Connectivities::Connectivities(const GroupElements<const Elem>& eG) {
    // Preliminar checks.
    if (eG.sizeOf<ElemI>()) {
        throw Error("Map Group only accepts ElemR elements");
    }

    // Removes elements with no physical model associated.
    ElemRGroup elem = eG;
    elem.removeMatId(MatId(0));

    // Generates graph.
    graph_ = GraphVertices<ElemR,CoordR3>(elem);
    for (UInt i = 0; i < graph_.numElems(); i++) {
        const GraphElem* graphElem = graph_.elem(i);
        ElementId id = graphElem->elem()->getId();
        index_.insert(pair<ElementId,const GraphElem*>(id, graphElem));
    }
}

Face Connectivities::getNeighFace(const Face& face) const {
    const ElemR* elem = face.first;
    const GraphElem* local = index_.find(elem->getId())->second;
    vector<const Coord*> localSideV = elem->getSideCoordinates(face.second);

    return getMatchingFace_(local, localSideV);
}

const SurfR* Connectivities::getNeighSurf(const Face& face) const {
    const ElemR* elem = face.first;
    vector<const Coord*> localSideV = elem->getSideCoordinates(face.second);
    const GraphElem* local = index_.find(elem->getId())->second;

    for (UInt i = 0; i < local->numNeighbors(); i++) {
        const ElemR* neigh = local->getNeighbor(i)->elem();
        vector<const Coord*> neighV = neigh->getCoordinates();
        if (ElementBase::areSameCoords(localSideV, neighV)) {
            return neigh;
        }
    }
    return NULL;
}

Face Connectivities::getInnerFace(const SurfR& surf) const {
    const GraphElem* local = index_.find(surf.getId())->second;
    vector<const Coord*> localV = local->elem()->getCoordinates();
    Face face = getMatchingFace_(local, localV);
    CVecR3 faceNormal = face.first->getSideNormal(face.second);
    if (surf.getNormal() == faceNormal) {
        return face;
    } else {
        return getNeighFace(face);
    }
}

Face Connectivities::getOuterFace(const SurfR& surf) const {
    return getNeighFace(getInnerFace(surf));
}

bool Connectivities::isDomainBoundary(const SurfR& surf) const {
    return (getInnerFace(surf).first == NULL
            || getOuterFace(surf).first == NULL);
}

bool Connectivities::isDomainBoundary(Face face) const {
    return getNeighFace(face).first == NULL;
}

Face Connectivities::getMatchingFace_(
        const GraphElem* local,
        const vector<const Coord*> localSideV) const {
    for (UInt i = 0; i < local->numNeighbors(); i++) {
        const ElemR* neigh = local->getNeighbor(i)->elem();
        for (UInt f = 0; f < neigh->numberOfFaces(); f++) {
            vector<const Coord*> neighSideV = neigh->getSideCoordinates(f);
            if (ElementBase::areSameCoords(localSideV, neighSideV)) {
                return Face(neigh, f);
            }
        }
    }
    return Face(NULL, 0);
}
