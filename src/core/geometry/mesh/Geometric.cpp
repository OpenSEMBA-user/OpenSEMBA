

#include "Geometric.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {

Geometric::Geometric(const Grid3& grid)
:   grid_(grid) {

}

Geometric::Geometric(const Grid3& grid,
                     const CoordR3Group& cG,
                     const Element::Group<ElemR>& elem,
                     const LayerGroup& layers)
:   Unstructured(cG, elem, layers),
    grid_(grid) {

}

Geometric::Geometric(const Geometric& rhs)
:   Unstructured(rhs),
    grid_(rhs.grid_) {

}

Geometric::Geometric(const Grid3& grid, const Unstructured& unstructured) :
	Unstructured(unstructured),
	grid_(grid) {}

Geometric& Geometric::operator=(const Geometric& rhs) {
    if(this == &rhs) {
        return *this;
    }
    Unstructured::operator=(rhs);
    grid_ = rhs.grid_;

    return *this;
}

Structured* Geometric::getMeshStructured(const Math::Real tol) const {
    return Unstructured::getMeshStructured(grid_, tol);
}

void Geometric::applyScalingFactor(const Math::Real factor) {
    Unstructured::applyScalingFactor(factor);
    grid_.applyScalingFactor(factor);
}

} /* namespace Mesh */
} /* namespace Geometry */
} /* namespace SEMBA */
