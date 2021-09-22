

#include "Geometric.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {

Geometric::Geometric() {

}

Geometric::Geometric(const Grid3& grid)
:   grid_(grid) {

}

Geometric::Geometric(const Grid3& grid,
                     const Coordinate::Group<const CoordR3>& cG,
                     const Element::Group<const ElemR>& elem,
                     const Layer::Group<const Layer::Layer>& layers)
:   Unstructured(cG, elem, layers),
    grid_(grid) {

}

Geometric::Geometric(const Geometric& rhs)
:   Unstructured(rhs),
    grid_(rhs.grid_) {

}

Geometric::~Geometric() {

}

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

void Geometric::printInfo() const {
    std::cout << " --- Mesh geometric Info --- " << std::endl;
    Unstructured::printInfo();
    grid_.printInfo();
}

} /* namespace Mesh */
} /* namespace Geometry */
} /* namespace SEMBA */
