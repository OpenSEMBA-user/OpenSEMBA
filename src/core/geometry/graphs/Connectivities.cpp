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

Connectivities::Connectivities(const GroupElements<const ElemR>& eG) {
    // Generates graph.
    graph_.init(eG);
    for (UInt i = 0; i < graph_.numElems(); i++) {
        GraphElem* graphElem = graph_.elem(i);
        ElementId id = graphElem->elem()->getId();
        index_.insert(pair<ElementId,const GraphElem*>(id, graphElem));
    }
}

Face Connectivities::getNeighFace(const Face& face) const {
    const ElemR* elem = face.first;
    const GraphElem* local = index_.find(elem->getId())->second;
    vector<const CoordR3*> localSideV = elem->getSideCoordinates(face.second);

    return getMatchingFace_(local, localSideV);
}

const SurfR* Connectivities::getNeighSurf(const Face& face) const {
    const ElemR* elem = face.first;
    vector<const CoordR3*> localSideV = elem->getSideCoordinates(face.second);
    const GraphElem* local = index_.find(elem->getId())->second;

    for (UInt i = 0; i < local->numNeighbors(); i++) {
        const ElemR* neigh = local->getNeighbor(i)->elem();
        if (neigh->is<SurfR>()) {
            vector<const CoordR3*> neighV = neigh->getCoordinates();
            if (ElementBase::areSameCoords<CoordR3,CoordR3>(
                    localSideV, neighV)) {
                return neigh->castTo<SurfR>();
            }
        }
    }
    return NULL;
}

Face Connectivities::getInnerFace(const SurfR* surf) const {
    const GraphElem* local = index_.find(surf->getId())->second;
    vector<const CoordR3*> localV = local->elem()->getCoordinates();
    Face face = getMatchingFace_(local, localV);
    CVecR3 faceNormal = face.first->getSideNormal(face.second);
    if ((surf->getNormal() == faceNormal) || isDomainBoundary(surf)) {
        return face;
    } else {
        return getNeighFace(face);
    }
}

Face Connectivities::getOuterFace(const SurfR* surf) const {
    return getNeighFace(getInnerFace(surf));
}

bool Connectivities::isDomainBoundary(const SurfR* surf) const {
    return (getInnerFace(surf).first == NULL
            || getOuterFace(surf).first == NULL);
}

bool Connectivities::isDomainBoundary(Face face) const {
    return getNeighFace(face).first == NULL;
}

Face Connectivities::getMatchingFace_(
        const GraphElem* local,
        const vector<const CoordR3*> localSideV) const {
    for (UInt i = 0; i < local->numNeighbors(); i++) {
        const GraphElem* neighConn = local->getNeighbor(i);
        if (neighConn->elem() == NULL) {
            return Face(NULL, 0);
        }
        const ElemR* neigh = neighConn->elem();
        if (neigh->is<VolR>()) {
            for (UInt f = 0; f < neigh->numberOfFaces(); f++) {
                vector<const CoordR3*> neighSideV = neigh->getSideCoordinates(f);
                if (ElementBase::areSameCoords(localSideV, neighSideV)) {
                    return Face(neigh->castTo<VolR>(), f);
                }
            }
        }
    }
    return Face(NULL, 0);
}

UInt Connectivities::size() const {
    return graph_.numElems();
}

bool Connectivities::existsReciprocity() const {

    for (UInt i = 0; i < graph_.numElems(); i++) {
        const ElemR* elem = graph_.elem(i)->elem();
        // Checks volumic reciprocity in connectivities.
        if (elem->is<VolR>()) {
            const VolR* vol = elem->castTo<VolR>();
            for (UInt f = 0; f < vol->numberOfFaces(); f++) {
                Face local(vol, f);
                Face neigh = this->getNeighFace(local);
                if (this->isDomainBoundary(local)) {
                    if (neigh != Face(NULL, 0)) {
                        return false;
                    }
                } else {
                    Face neighNeigh = this->getNeighFace(neigh);
                    if (local != neighNeigh) {
                        return false;
                    }
                }
            }
        }
        // Checks surf reciprocity in connectivities.
        if (elem->is<SurfR>()) {
            const SurfR* surf = elem->castTo<SurfR>();
            Face inner = this->getInnerFace(surf);
            if (inner.first != NULL) {
                Face outer = this->getOuterFace(surf);
                Face inNeigh = this->getNeighFace(inner);
                if (outer != inNeigh) {
                    return false;
                }
            }
        }
    }
    return true;
}
