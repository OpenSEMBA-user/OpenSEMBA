

#include "Structured.h"
#include "Unstructured.h"

#include "geometry/element/Tetrahedron.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {

Unstructured::Unstructured(const CoordR3Group& cG,
                           const Element::Group<const ElemR>& elem,
                           const LayerGroup& layers) :   
    coords_(cG),
    elems_(elem.cloneElems()),
    layers_(layers) 
{
    elems_.reassignPointers( coords_ );
    elems_.reassignPointers( layers_ );
}

Unstructured::Unstructured(const Unstructured& rhs) :   
    coords_(rhs.coords()),
    elems_(rhs.elems().cloneElems()),
    layers_(rhs.layers()) 
{
    elems_.reassignPointers(this->coords());
    elems_.reassignPointers(this->layers());
}

Unstructured& Unstructured::operator=(const Unstructured& rhs) 
{
    coords_ = rhs.coords();
    elems_ = rhs.elems().cloneElems();
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

    std::vector<ElemI*> newElems;
    newElems.reserve(elems().size());
    for (std::size_t i = 0; i < elems().size(); i++) {
		ElemI* newElem = elems()(i)->toStructured(res->coords(), grid, tol);
        if (newElem != nullptr) {
            newElems.push_back(newElem);
        }
    }
    res->elems().add(newElems);
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
