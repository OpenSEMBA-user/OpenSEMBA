

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

void Structured::reassignPointers(
    const SEMBA::Group::Identifiable<Element::Model, MatId>& matGr) {
    elems_.reassignPointers(this->coords());
    elems_.reassignPointers(this->layers());
    if (!matGr.empty()) {
        elems_.reassignPointers(matGr);
    }
}

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
