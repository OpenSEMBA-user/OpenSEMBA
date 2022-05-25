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
              const CoordR3Group& cG,
              const Element::Group<ElemR>& elem,
              const LayerGroup& = LayerGroup());
    Geometric(const Geometric&);
    Geometric(Geometric&&) = default;
    virtual ~Geometric() = default;

    Geometric& operator=(const Geometric& rhs);
    Geometric& operator=(Geometric&&) = default;

    virtual std::unique_ptr<Mesh> clone() const override {
        return std::make_unique<Geometric>(*this);
    }

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

