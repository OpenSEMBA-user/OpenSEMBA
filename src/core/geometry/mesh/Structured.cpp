

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
        const CoordI3Group& cG,
        const ElemIGroup& elem,
        const LayerGroup& layers,
        const BoundTerminations3& bounds) :   
    grid_(grid),
    coords_(cG),
    elems_(elem),
    layers_(layers),
    bounds_(bounds) 
{
    elems_.reassignPointers(this->coords());
    elems_.reassignPointers(this->layers());
}

Structured::Structured(const Structured& rhs)
:   grid_(rhs.grid_),
    coords_(rhs.coords_),
    elems_(rhs.elems()),
    layers_(rhs.layers_),
    bounds_(rhs.bounds_) {

    elems_.reassignPointers(this->coords());
    elems_.reassignPointers(this->layers());
}

Structured& Structured::operator=(const Structured& rhs) 
{
    grid_ = rhs.grid_;
    coords_ = rhs.coords();
    elems_ = rhs.elems();
    layers_ = rhs.layers();
    bounds_ = rhs.bounds();

    elems_.reassignPointers(this->coords());
    elems_.reassignPointers(this->layers());

    return *this;
}

void Structured::applyScalingFactor(const Math::Real factor) {
    grid_.applyScalingFactor(factor);
}

Unstructured* Structured::getMeshUnstructured() const 
{
    Unstructured* res = new Unstructured;

    for (auto const& coord : coords()) {
        auto newCoord = std::make_unique<CoordR3>(*coord->toUnstructured(grid_));
        res->coords().add(newCoord);
    }

    for (auto const& elem : elems()) {
        auto newElem = elem->toUnstructured(res->coords(), grid_);
        res->elems().add(newElem);
    }

    res->layers() = layers();
    return res;
}

void Structured::reassignPointers(const PMGroup& matGr)
{
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

ElemView Structured::reassign(const ElemView& inGroup) {
    ElemView res;

    res.reserve(inGroup.size());
    for (const auto& elem : inGroup) {
        res.push_back(elems_.getId(
            elem->getId()
        ));
    }

    return res;
}

} /* namespace Mesh */
} /* namespace Geometry */
} /* namespace SEMBA */
