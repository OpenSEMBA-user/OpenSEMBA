

#include "Structured.h"
#include "Unstructured.h"

#include "geometry/element/Tetrahedron.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {

Unstructured::Unstructured(const CoordR3Group& cG,
                           const ElemRGroup& elem,
                           const LayerGroup& layers) :   
    coords_(cG),
    elems_(elem),
    layers_(layers) 
{
    elems_.reassignPointers( coords_ );
    elems_.reassignPointers( layers_ );
}

Unstructured::Unstructured(const Unstructured& rhs) :   
    coords_(rhs.coords()),
    elems_(rhs.elems()),
    layers_(rhs.layers()) 
{
    elems_.reassignPointers(this->coords());
    elems_.reassignPointers(this->layers());
}

Unstructured& Unstructured::operator=(const Unstructured& rhs) 
{
    coords_ = rhs.coords();
    elems_ = rhs.elems();
    layers_ = rhs.layers();

    elems_.reassignPointers(this->coords());
    elems_.reassignPointers(this->layers());

    return *this;
}

Structured* Unstructured::getMeshStructured(const Grid3& grid, const Math::Real tol) const 
{
    Structured* res = new Structured(grid);

    for (auto const& coord : coords()) {
        auto newCoord = std::make_unique<CoordI3>(*coord->toStructured(grid));
        res->coords().add(newCoord);
    }

    for (auto const& elem : elems()) {
        auto newElem = elem->toStructured(res->coords(), grid, tol);
        res->elems().add(newElem);
    }

    res->layers() = layers();
    return res;
}

void Unstructured::applyScalingFactor(const Math::Real factor) {
    coords_.applyScalingFactor(factor);
}

void Unstructured::reassignPointers(const PMGroup& matGr) 
{
    elems_.reassignPointers(this->coords());
    elems_.reassignPointers(this->layers());
    if (!matGr.empty()) {
        elems_.reassignPointers(matGr);
    }
}

BoxR3 Unstructured::getBoundingBox() const {
    return elems().getBound();
}

ElemView Unstructured::reassign(const ElemView& inGroup) {
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
