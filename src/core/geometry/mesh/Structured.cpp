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

#include "Unstructured.h"
#include "Structured.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {

Structured::Structured(const Grid3& grid)
:   grid_(grid) {

}

Structured::Structured(
        const Grid3& grid,
        const Coordinate::Group<const CoordI3>& cG,
        const Element::Group<const ElemI>& elem,
        const Layer::Group<const Layer::Layer>& layers,
        const BoundTerminations3& bounds)
:   grid_(grid),
    coords_(cG.cloneElems()),
    elems_(elem.cloneElems()),
    layers_(layers.cloneElems()),
    bounds_(bounds) {

    elems_.reassignPointers(this->coords());
    elems_.reassignPointers(this->layers());
}

Structured::Structured(const Structured& rhs)
:   grid_(rhs.grid_),
    coords_(rhs.coords().cloneElems()),
    elems_(rhs.elems().cloneElems()),
    layers_(rhs.layers().cloneElems()),
    bounds_(rhs.bounds()) {

    elems_.reassignPointers(this->coords());
    elems_.reassignPointers(this->layers());
}

Structured::~Structured() {

}

Structured& Structured::operator=(const Structured& rhs) {
    if(this == &rhs) {
        return *this;
    }

    grid_ = rhs.grid_;
    coords_ = rhs.coords().cloneElems();
    elems_ = rhs.elems().cloneElems();
    layers_ = rhs.layers().cloneElems();
    bounds_ = rhs.bounds();

    elems_.reassignPointers(this->coords());
    elems_.reassignPointers(this->layers());

    return *this;
}

void Structured::applyScalingFactor(const Math::Real factor) {
    grid_.applyScalingFactor(factor);
}

Unstructured* Structured::getMeshUnstructured() const {
    Unstructured* res = new Unstructured;

    std::vector<CoordR3*> newCoords;
    newCoords.reserve(coords().size());
    for (std::size_t i = 0; i < coords().size(); i++) {
        CoordR3* newCoord = coords()(i)->toUnstructured(grid_);
        if (newCoord != nullptr) {
            newCoords.push_back(newCoord);
        }
    }
    res->coords().add(newCoords);

    std::vector<ElemR*> newElems;
    newElems.reserve(elems().size());
    for (std::size_t i = 0; i < elems().size(); i++) {
        ElemR* newElem = elems()(i)->toUnstructured(res->coords(), grid_);
        if (newElem != nullptr) {
            newElems.push_back(newElem);
        }
    }
    res->elems().add(newElems);
    res->layers() = layers().cloneElems();
    return res;
}

//Structured* Structured::getConnectivityMesh() const {
//    Structured* res = new Structured(grid());
//    res->coords() = coords().cloneElems();
//    Element::Group<const ElemI> elems = this->elems();
//    elems.removeMatId(MatId(0));
//    Graph::Vertices<ElemI, CoordI3> graphLayer;
//    graphLayer.init(elems);
//    std::vector<std::vector<const ElemI*>> comps =
//        graphLayer.getConnectedComponents();
//    for (std::size_t c = 0; c < comps.size(); c++) {
//        std::stringstream layerName;
//        layerName << "Component " << c+1;
//        Layer::Layer* newLayer = res->layers().addId(
//                                     new Layer::Layer(layerName.str()))(0);
//        std::vector<ElemI*> newElemsLayer;
//        newElemsLayer.resize(comps[c].size());
//        for (std::size_t e = 0; e < comps[c].size(); e++) {
//            newElemsLayer[e] = comps[c][e]->cloneTo<ElemI>();
//            newElemsLayer[e]->setLayer(newLayer);
//        }
//        res->elems().add(newElemsLayer);
//    }
//    res->reassignPointers(res->coords());
//    res->reassignPointers(res->layers());
//    return res;
//}

void Structured::reassignPointers(
    const SEMBA::Group::Identifiable<Element::Model, MatId>& matGr) {
    elems_.reassignPointers(this->coords());
    elems_.reassignPointers(this->layers());
    if (!matGr.empty()) {
        elems_.reassignPointers(matGr);
    }
}

void Structured::printInfo() const {
    std::cout << " --- Mesh structured info --- " << std::endl;
    grid_.printInfo();
    std::cout << "Number of coordinates: " << coords().size() << std::endl;
    std::cout << "Number of elements: " << elems().size() << std::endl;
    layers().printInfo();
}

//void Structured::convertToHex(Element::Group<const SurfI> surfs) {
//    std::vector<HexI8*> hexes = discretizeWithinBoundary(surfs);
//    this->elems_.removeId(surfs.getIds());
//    elems().addId(hexes);
//}
//
//void Structured::addAsHex(Element::Group<const VolR> region) {
//    std::vector<Math::CVecR3> center =
//        getCenterOfCellsInside(region.getBound());
//    std::vector<HexI8*> hexes;
//    hexes.reserve(center.size());
//    for (std::size_t i = 0; i < center.size(); i++) {
//        for (std::size_t j = 0; j < region.size(); j++) {
//            if (region(j)->isInnerPoint(center[i])) {
//                ElemId id = region(j)->getId();
//                BoxI3 box = getBoxIContaining(center[i]);
//                const Layer::Layer* lay = region(j)->getLayer();
//                const Element::Model* mat = region(j)->getModel();
//				hexes.push_back(new HexI8(coords(), id, box, lay, mat));
//                break;
//            }
//        }
//    }
//    elems().add(hexes);
//}

//std::vector<HexI8*> Structured::discretizeWithinBoundary(
//        const Element::Group<const SurfI>& surf) {
//    const std::vector<std::pair<const SurfI*, const SurfI*>> pair =
//            getPairsDefiningVolumeWithin(surf);
//    std::vector<HexI8*> res;
//    res.reserve(pair.size());
//    for (std::size_t i = 0; i < pair.size(); i++) {
//        ElemId id(0);
//        BoxI3 box(*pair[i].first->getMinV(), *pair[i].second->getMaxV());
//        const Layer::Layer* lay = pair[i].first->getLayer();
//        if (pair[i].first->getLayerId() != pair[i].second->getLayerId()) {
//            throw Error::InvalidBoundary();
//        }
//        const Element::Model* mat = pair[i].first->getModel();
//        if (pair[i].first->getMatId() != pair[i].second->getMatId()) {
//            throw Error::InvalidBoundary();
//        }
//        res.push_back(new HexI8(coords(), id, box, lay, mat));
//    }
//    return res;
//}
//
//std::vector<std::pair<const SurfI*, const SurfI*> >
//    Structured::getPairsDefiningVolumeWithin(
//        const Element::Group<const SurfI>& origBound) const {
//    std::vector<std::pair<const SurfI*, const SurfI*> > res;
//    const std::size_t nOrigBound = origBound.size();
//    // Checks that bound.size is an even number.
//    if (nOrigBound % 2 != 0) {
//        throw Error::InvalidBoundary();
//    }
//    // Remove Surfaces not lying in a xy plane.
//    std::vector<const SurfI*> bound;
//    bound.reserve(nOrigBound);
//    for (std::size_t b = 0; b < origBound.size(); b++) {
//        if (origBound(b)->isContainedInPlane(Math::Constants::xy)) {
//            bound.push_back(origBound(b));
//        }
//    }
//    // Sort remaining quad Ids using coordinates as a lexicographical order.
//    // Pairs quadrilaterals that are aligned in the same Z natural axe.
//    const std::size_t nBound = bound.size();
//    assert(nBound % 2 == 0);
//    res.reserve(nBound / 2);
//    // Sorts.
//    Math::Matrix::Dynamic<Math::Int> quads(nBound, 4);
//    for (std::size_t b = 0; b < nBound; b++) {
//        Math::CVecI3 minPos = bound[b]->getMinV()->pos();
//        // Stores boundary at quad list.
//        quads(b, 0) = (Math::Int) bound[b]->getId().toInt();
//        for (std::size_t i = 0; i < 3; i++) {
//            quads(b, i+1) = minPos(i);
//        }
//    }
//    quads.sortRows_omp(1,3);
//    // Performs pairing.
//    std::pair<const SurfI*, const SurfI*> aux;
//    for (std::size_t b = 0; b < nBound; b++) {
//        const ElemId id(quads(b,0));
//        if (b % 2 == 0) {
//            aux.first = elems().getId(id)->castTo<SurfI>();
//        } else {
//            aux.second = elems().getId(id)->castTo<SurfI>();
//            res.push_back(aux);
//        }
//    }
//    return res;
//}

Math::Real Structured::getMinimumSpaceStep() const {
    return grid_.getMinimumSpaceStep();
}

BoxR3 Structured::getBoundingBox() const {
    return grid_.getFullDomainBoundingBox();
}

void Structured::reassign( Element::Group<const Elem>& inGroup ) {
    Element::Group<const Elem> res;
    for (std::size_t i = 0; i < inGroup.size(); i++) {
        ElemId id = inGroup(i)->getId();
        res.add(this->elems().getId(id));
    }
    inGroup = res;
}

} /* namespace Mesh */
} /* namespace Geometry */
} /* namespace SEMBA */
