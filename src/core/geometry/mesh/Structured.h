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
               const ElemIGroup& elem,
               const LayerGroup& = LayerGroup(), 
               const BoundTerminations3& = BoundTerminations3());
    Structured(const Structured&);
    Structured(Structured&&) = default;
    virtual ~Structured() = default;

    Structured& operator=(const Structured&);
    Structured& operator=(Structured&&) = default;

    virtual std::unique_ptr<Mesh> clone() const override {
        return std::make_unique<Structured>(*this);
    }

    BoundTerminations3&         bounds() { return bounds_; }
    Grid3&                      grid  () { return grid_; }
    CoordI3Group& coords() { return coords_; }
    ElemIGroup&      elems () { return elems_; }
    LayerGroup&             layers() { return layers_; }

    const BoundTerminations3&         bounds() const { return bounds_; }
    const Grid3&                      grid  () const { return grid_; }
    const CoordI3Group& coords() const { return coords_; }
    const ElemIGroup&      elems () const { return elems_; }
    const LayerGroup&             layers() const { return layers_; }

    Unstructured* getMeshUnstructured() const;

    Math::Real getMinimumSpaceStep() const;
    void applyScalingFactor(const Math::Real factor);
    BoxR3 getBoundingBox() const;

    void reassignPointers(const PMGroup& matGr = PMGroup());

    virtual ElemView reassign(const ElemView&);

private:
    Grid3 grid_;
	CoordI3Group coords_;
    ElemIGroup elems_;
    LayerGroup layers_;
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

