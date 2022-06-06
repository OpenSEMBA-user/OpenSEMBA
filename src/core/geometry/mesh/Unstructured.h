#pragma once

#include "geometry/element/Triangle.h"


#include "Mesh.h"
#include "geometry/Grid.h"
#include "geometry/coordinate/Group.h"
#include "geometry/element/Group.h"
#include "geometry/layer/Group.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {

class Structured;

class Unstructured : public Mesh {
public:
    Unstructured() = default;
    Unstructured(const CoordR3Group& cG,
                 const ElemRGroup& elem,
                 const LayerGroup& = LayerGroup());
    Unstructured(const Unstructured& param);
    Unstructured(Unstructured&&) = default;
    virtual ~Unstructured() = default;

    Unstructured& operator=(const Unstructured&);
    Unstructured& operator=(Unstructured&&) = default;

    virtual std::unique_ptr<Mesh> clone() const override {
        return std::make_unique<Unstructured>(*this);
    }

    CoordR3Group& coords() { return coords_; }
    ElemRGroup&      elems () { return elems_; }
    LayerGroup& layers() { return layers_; }

    const CoordR3Group& coords() const { return coords_; }
    const ElemRGroup&      elems () const { return elems_; }
    const LayerGroup& layers() const { return layers_; }

    Structured* getMeshStructured(
            const Grid3& grid,
            const Math::Real tol = Grid3::tolerance) const;

    void applyScalingFactor(const Math::Real factor);
    BoxR3 getBoundingBox() const;

    void reassignPointers(const PMGroup& matGr = PMGroup());

    virtual ElemView reassign(const ElemView&);

private:
	CoordR3Group coords_;
    ElemRGroup elems_;
	LayerGroup layers_;
};

} /* namespace Mesh */
} /* namespace Geometry */
} /* namespace SEMBA */

