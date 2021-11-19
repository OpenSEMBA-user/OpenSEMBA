

#pragma once

#include "Unstructured.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {

class Geometric : public Unstructured {
public:
    Geometric() = default;
    Geometric(const Grid3& grid);
    Geometric(const Grid3& grid,
              const Coordinate::Group<CoordR3>& cG,
              const Element::Group<const ElemR>& elem,
              const Layer::Group<const Layer::Layer>& = Layer::Group<const Layer::Layer>());
    Geometric(const Geometric& param);
    virtual ~Geometric() = default;

    Geometric& operator=(const Geometric& rhs);

    SEMBA_CLASS_DEFINE_CLONE(Geometric);

    Grid3&       grid()       { return grid_; }
    const Grid3& grid() const { return grid_; }

    Structured* getMeshStructured(const Math::Real = Grid3::tolerance) const;

    void applyScalingFactor(const Math::Real factor);

private:
	Grid3 grid_;
};

} /* namespace Mesh */
} /* namespace Geometry */
} /* namespace SEMBA */

