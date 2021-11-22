

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
    Unstructured(const Coordinate::Group<CoordR3>& cG,
                 const Element::Group<const ElemR>& elem,
                 const Layer::Group<const Layer::Layer>& =
                     Layer::Group<const Layer::Layer>());
    Unstructured(const Unstructured& param);
    Unstructured(Unstructured&&) noexcept;
    virtual ~Unstructured() = default;

    Unstructured& operator=(const Unstructured&);
    Unstructured& operator=(Unstructured&&) noexcept;

    SEMBA_CLASS_DEFINE_CLONE(Unstructured);

    Coordinate::Group<CoordR3>& coords() { return coords_; }
    Element::Group<ElemR>&      elems () { return elems_; }
    Layer::Group<Layer::Layer>& layers() { return layers_; }

    const Coordinate::Group<CoordR3>& coords() const { return coords_; }
    const Element::Group<ElemR>&      elems () const { return elems_; }
    const Layer::Group<Layer::Layer>& layers() const { return layers_; }

    Structured* getMeshStructured(
            const Grid3& grid,
            const Math::Real tol = Grid3::tolerance) const;

    void applyScalingFactor(const Math::Real factor);
    BoxR3 getBoundingBox() const;

    void reassignPointers(
        const SEMBA::Group::Identifiable<Element::Model, MatId>& =
            SEMBA::Group::Identifiable<Element::Model, MatId>());

    virtual void reassign( Element::Group<const Elem>& );

private:
	Coordinate::Group<CoordR3> coords_;
	Element::Group<ElemR> elems_;
	Layer::Group<Layer::Layer> layers_;
};

} /* namespace Mesh */
} /* namespace Geometry */
} /* namespace SEMBA */

