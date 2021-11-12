

#include "Structured.h"
#include "Unstructured.h"

#include "geometry/element/Tetrahedron.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {

Unstructured::Unstructured() {

}

Unstructured::Unstructured(const Coordinate::Group<const CoordR3>& cG,
                           const Element::Group<const ElemR>& elem,
                           const Layer::Group<const Layer::Layer>& layers)
:   coords_(cG.cloneElems()),
    elems_(elem.cloneElems()),
    layers_(layers.cloneElems()) {

    elems_.reassignPointers( coords_ );
    elems_.reassignPointers( layers_ );
}

Unstructured::Unstructured(const Unstructured& rhs)
:   coords_(rhs.coords().cloneElems()),
    elems_(rhs.elems().cloneElems()),
    layers_(rhs.layers().cloneElems()) {

    elems_.reassignPointers(this->coords());
    elems_.reassignPointers(this->layers());
}

Unstructured& Unstructured::operator=(const Unstructured& rhs) {
    if(this == &rhs) {
        return *this;
    }

    coords_ = rhs.coords().cloneElems();
    elems_ = rhs.elems().cloneElems();
    layers_ = rhs.layers().cloneElems();

    elems_.reassignPointers(this->coords());
    elems_.reassignPointers(this->layers());

    return *this;
}

Structured* Unstructured::getMeshStructured(const Grid3& grid,
        const Math::Real tol) const {
    Structured* res = new Structured(grid);

    std::vector<CoordI3*> newCoords;
    newCoords.reserve(coords().size());
    for (std::size_t i = 0; i < coords().size(); i++) {
        CoordI3* newCoord = coords()(i)->toStructured(grid);
        if (newCoord != nullptr) {
            newCoords.push_back(newCoord);
        }
    }
    res->coords().add(newCoords);

    std::vector<ElemI*> newElems;
    newElems.reserve(elems().size());
    for (std::size_t i = 0; i < elems().size(); i++) {
		ElemI* newElem = elems()(i)->toStructured(res->coords(), grid, tol);
        if (newElem != nullptr) {
            newElems.push_back(newElem);
        }
    }
    res->elems().add(newElems);
    res->layers() = layers().cloneElems();
    return res;
}

//Unstructured* Unstructured::getConnectivityMesh() const {
//    Unstructured* res = new Unstructured;
//    res->coords() = coords().cloneElems();
//    Element::Group<const ElemR> elems = this->elems();
//    elems.removeMatId(MatId(0));
//    Graph::Vertices<ElemR, CoordR3> graphLayer;
//    graphLayer.init(elems);
//    std::vector<std::vector<const ElemR*>> comps =
//        graphLayer.getConnectedComponents();
//    for (std::size_t c = 0; c < comps.size(); c++) {
//        std::stringstream layerName;
//        layerName << "Component " << c+1;
//        Layer::Layer* newLayer =
//            res->layers_.addId(new Layer::Layer(layerName.str()))(0);
//        std::vector<ElemR*> newElemsLayer;
//        //newElemsLayer.resize(comps[c].size());
//        //for (std::size_t e = 0; e < comps[c].size(); e++) {
//        //    newElemsLayer[e] = comps[c][e]->cloneTo<ElemR>();
//        //    newElemsLayer[e]->setLayer(newLayer);
//        //}
//        //res->elems().add(newElemsLayer);
//    }
//    res->reassignPointers(res->coords());
//    res->reassignPointers(res->layers());
//    return res;
//}
//
//std::vector<Element::Face> Unstructured::getBorderWithNormal(
//        const std::vector<Element::Face>& border,
//        const Math::CVecR3& normal) {
//    const std::size_t nK = border.size();
//    std::vector<Element::Face> res;
//    res.reserve(nK);
//    for (std::size_t i = 0; i < nK; i++) {
//        const VolR* tet = border[i].first;
//        const std::size_t face = border[i].second;
//        Math::CVecR3 tetNormal = tet->getSideNormal(face);
//        if (tetNormal == normal && !tet->isCurvedFace(face)) {
//            res.push_back(border[i]);
//        }
//    }
//    return res;
//}
//
////Element::Group<const Tri> Unstructured::convertToTri(
////        const Element::Group<const ElemR>& region,
////        bool includeTets) const {
////
////    Element::Group<Tri> res(region.cloneElems());
////    if (includeTets) {
////        Element::Group<const Tet> tet = region.getOf<Tet>();
////        std::vector<Element::Face> border = getInternalBorder(tet);
////        for (std::size_t i = 0; i < border.size(); i++) {
////            if (border[i].first->is<Tet>()) {
////                const Tet* tet = border[i].first->castTo<Tet>();
////                const std::size_t face = border[i].second;
////                res.addId(tet->getTri3Face(face));
////            }
////        }
////    }
////    return Element::Group<const Tri>(res);
////}
//
//std::vector<Element::Face> Unstructured::getInternalBorder(
//        const Element::Group<const ElemR>& region) const {
//    Graph::Connectivities conn(region);
//    std::vector<Element::Face> res;
//    res.reserve(region.size());
//    const Element::Group<const VolR> vol = region.getOf<VolR>();
//    for (std::size_t i = 0; i < vol.size(); i++) {
//        for (std::size_t f = 0; f < vol(i)->numberOfFaces(); f++) {
//            Element::Face face(vol(i), f);
//            conn.isDomainBoundary(face);
//            res.push_back(face);
//        }
//    }
//    return res;
//}
//
//std::vector<Element::Face> Unstructured::getExternalBorder(
//        const Element::Group<const ElemR>& region) const {
//    const Graph::Connectivities conn(elems());
//    std::vector<Element::Face> internal = getInternalBorder(region);
//    std::vector<Element::Face> external;
//    external.reserve(internal.size());
//    for (std::size_t i = 0; i < internal.size(); i++) {
//        if (!conn.isDomainBoundary(internal[i]))  {
//            external.push_back(conn.getNeighFace(internal[i]));
//        }
//    }
//    return external;
//}
//
//Element::Group <const VolR> Unstructured::getAdjacentRegion(
//        const Element::Group<const VolR>& region) const {
//    std::vector<Element::Face> outer = getExternalBorder(region);
//    std::size_t nOut = outer.size();
//    // Removes repeated.
//    Math::Matrix::Dynamic<std::size_t> aux(nOut,1);
//    for (std::size_t i = 0; i < nOut; i++) {
//        aux(i,0) = outer[i].first->getId().toInt();
//    }
//    aux.sortAndRemoveRepeatedRows_omp();
//    // Prepares result.
//    Element::Group<ElemR> res;
//    for (std::size_t i = 0; i < aux.nRows(); i++) {
//        res.add(elems().getId(ElemId(aux(i,0)))->cloneTo<ElemR>());
//    }
//    return res;
//}
//
//Element::Group<const SurfR> Unstructured::getMaterialBoundary(
//        const MatId matId,
//        const LayerId layId) const {
//
//    return elems().getMatLayerId(matId, layId).getOf<SurfR>();
//}
//
void Unstructured::applyScalingFactor(const Math::Real factor) {
    coords_.applyScalingFactor(factor);
}

void Unstructured::reassignPointers(
    const SEMBA::Group::Identifiable<Element::Model, MatId>& matGr) {
        elems_.reassignPointers(this->coords());
        elems_.reassignPointers(this->layers());
    if (!matGr.empty()) {
        elems_.reassignPointers(matGr);
    }
}

BoxR3 Unstructured::getBoundingBox() const {
    return elems().getBound();
}

void Unstructured::reassign( Element::Group<const Elem>& inGroup ) {
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
