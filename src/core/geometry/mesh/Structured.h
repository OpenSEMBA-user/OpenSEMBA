

#pragma once

#include <exception>

#include "geometry/element/Hexahedron8.h"

#include "Mesh.h"
#include "geometry/Grid.h"
#include "geometry/BoundTerminations.h"
#include "geometry/coordinate/Group.h"
#include "geometry/element/Group.h"
#include "geometry/layer/Group.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {

class Unstructured;

class Structured : public virtual Mesh {
public:
    Structured() = default;
    Structured(const Grid3& grid);
    Structured(const Grid3& grid,
               const CoordI3Group& cG,
               const Element::Group<const ElemI>& elem,
               const LayerGroup& = LayerGroup(), 
               const BoundTerminations3& = BoundTerminations3());
    Structured(const Structured&);
    Structured(Structured&&) = default;
    virtual ~Structured() = default;

    Structured& operator=(const Structured&);
    Structured& operator=(Structured&&) = default;

    SEMBA_CLASS_DEFINE_CLONE(Structured);

    BoundTerminations3&         bounds() { return bounds_; }
    Grid3&                      grid  () { return grid_; }
    CoordI3Group& coords() { return coords_; }
    Element::Group<ElemI>&      elems () { return elems_; }
    Layer::Group<>&             layers() { return layers_; }

    const BoundTerminations3&         bounds() const { return bounds_; }
    const Grid3&                      grid  () const { return grid_; }
    const CoordI3Group& coords() const { return coords_; }
    const Element::Group<ElemI>&      elems () const { return elems_; }
    const Layer::Group<>&             layers() const { return layers_; }

    Unstructured* getMeshUnstructured() const;

    Math::Real getMinimumSpaceStep() const;
    void applyScalingFactor(const Math::Real factor);
    BoxR3 getBoundingBox() const;
    void reassignPointers(
        const SEMBA::Group::Identifiable<Element::Model, MatId>& =
            SEMBA::Group::Identifiable<Element::Model, MatId>());

    virtual void reassign( Element::Group<const Elem>& );

private:
    Grid3 grid_;
	CoordI3Group coords_;
	Element::Group<ElemI> elems_;
	Layer::Group<> layers_;
	BoundTerminations3 bounds_;
};

namespace Error {
class InvalidBoundary : public std::exception {
public:
    InvalidBoundary() {}
    virtual ~InvalidBoundary() throw() {}

    const char* what() const throw() {
        return "Invalid boundary to discretize";
    }
};
} /* namespace Error */

} /* namespace Mesh */
} /* namespace Geometry */

typedef Geometry::Mesh::Structured StructuredMesh;

} /* namespace SEMBA */

