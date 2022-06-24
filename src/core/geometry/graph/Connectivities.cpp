#include "Connectivities.h"

namespace SEMBA {
namespace Geometry {
namespace Graph {

Connectivities::Connectivities(const std::vector<const ElemR*>& eG)
    : graph_(eG) 
{
    // Generates graph.
    for (std::size_t i = 0; i < graph_.numElems(); i++) {
        GraphElem* graphElem = graph_.elem(i);
        ElemId id = graphElem->elem()->getId();
        index_.insert(std::pair<ElemId,const GraphElem*>(id, graphElem));
    }
}

Face Connectivities::getNeighFace(const Face& face) const {
    const ElemR* elem = face.first;
    const GraphElem* local = index_.find(elem->getId())->second;
    std::vector<const CoordR3*> localSideV =
        elem->getSideCoordinates(face.second);

    return getMatchingFace_(local, localSideV);
}

const SurfR* Connectivities::getNeighSurf(const Face& face) const {
    const ElemR* elem = face.first;
    std::vector<const CoordR3*> localSideV =
        elem->getSideCoordinates(face.second);
    const GraphElem* local = index_.find(elem->getId())->second;

    for (std::size_t i = 0; i < local->numNeighbors(); i++) {
        const ElemR* neigh = local->getNeighbor(i)->elem();
        if (neigh->is<SurfR>()) {
            std::vector<const CoordR3*> neighV = neigh->getCoordinates();
            if (Geometry::Element::Base::areSameCoords<CoordR3,CoordR3>(
                    localSideV, neighV)) {
                return neigh->castTo<SurfR>();
            }
        }
    }
    return nullptr;
}

Face Connectivities::getInnerFace(const SurfR* surf) const {
    const GraphElem* local = index_.find(surf->getId())->second;
    std::vector<const CoordR3*> localV = local->elem()->getCoordinates();
    Face face = getMatchingFace_(local, localV);
    Math::CVecR3 faceNormal = face.first->getSideNormal(face.second);
    if ((surf->getNormal() == faceNormal) || isDomainBoundary(face)) {
        return face;
    } else {
        return getNeighFace(face);
    }
}

Face Connectivities::getOuterFace(const SurfR* surf) const {
    return getNeighFace(getInnerFace(surf));
}

bool Connectivities::isDomainBoundary(const SurfR* surf) const {
    return (getInnerFace(surf).first == nullptr
            || getOuterFace(surf).first == nullptr);
}

bool Connectivities::isDomainBoundary(Face face) const {
    if (face.first == nullptr) {
        return true;
    }
    return (getNeighFace(face).first == nullptr);
}

Face Connectivities::getMatchingFace_(
        const GraphElem* local,
        const std::vector<const CoordR3*> localSideV) const {
    for (std::size_t i = 0; i < local->numNeighbors(); i++) {
        const GraphElem* neighConn = local->getNeighbor(i);
        if (neighConn->elem() == nullptr) {
            return Face(nullptr, 0);
        }
        const ElemR* neigh = neighConn->elem();
        assert(neigh != nullptr);
        if (neigh->is<VolR>()) {
            for (std::size_t f = 0; f < neigh->numberOfFaces(); f++) {
                std::vector<const CoordR3*> neighSideV =
                    neigh->getSideCoordinates(f);
                if (Geometry::Element::Base::areSameCoords(localSideV,
                                                           neighSideV)) {
                    return Face(neigh->castTo<VolR>(), f);
                }
            }
        }
    }
    return Face(nullptr, 0);
}

std::size_t Connectivities::size() const {
    return graph_.numElems();
}

bool Connectivities::existsReciprocity() const {
    for (std::size_t i = 0; i < graph_.numElems(); i++) {
        const ElemR* elem = graph_.elem(i)->elem();
        // Checks volumic reciprocity in connectivities.
        if (elem->is<VolR>()) {
            const VolR* vol = elem->castTo<VolR>();
            for (std::size_t f = 0; f < vol->numberOfFaces(); f++) {
                Face local(vol, f);
                Face neigh = this->getNeighFace(local);
                if (this->isDomainBoundary(local)) {
                    if (neigh != Face(nullptr, 0)) {
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
            if (inner.first != nullptr) {
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

} /* namespace Graph */
} /* namespace Geometry */
} /* namespace SEMBA */
