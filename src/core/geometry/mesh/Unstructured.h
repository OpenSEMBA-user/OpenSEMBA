

#pragma once

#include "geometry/element/Triangle.h"
#include "geometry/graph/Connectivities.h"

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
    Unstructured();
    Unstructured(const Coordinate::Group<const CoordR3>& cG,
                 const Element::Group<const ElemR>& elem,
                 const Layer::Group<const Layer::Layer>& =
                     Layer::Group<const Layer::Layer>());
    Unstructured(const Unstructured& param);
    virtual ~Unstructured();

    Unstructured& operator=(const Unstructured& rhs);

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
    
	//Unstructured* getConnectivityMesh() const;
    //std::vector<Element::Face> getBorderWithNormal(
    //        const std::vector<Element::Face>& border, 
    //        const Math::CVecR3& normal);

    //bool isFloatingCoordinate(const CoordR3* coordinate) const;

    //Element::Group<const SurfR> getMaterialBoundary(const MatId   matId,
    //                                                const LayerId layId) const;
    //Element::Group<const SurfR> getSurfsMatching(
    //        const std::vector<Element::Face>& faces) const;
    //Element::Group<const Tri> convertToTri(
    //        const Element::Group<const ElemR>& region,
    //        bool ignoreTets) const;
    //std::vector<Element::Face> getInternalBorder(
    //        const Element::Group<const ElemR>& region) const;
    //std::vector<Element::Face> getExternalBorder(
    //        const Element::Group<const ElemR>& region) const;
    //Element::Group<const VolR> getAdjacentRegion(
    //        const Element::Group<const VolR>& region) const;
    void applyScalingFactor(const Math::Real factor);
    BoxR3 getBoundingBox() const;

    void reassignPointers(
        const SEMBA::Group::Identifiable<Element::Model, MatId>& =
            SEMBA::Group::Identifiable<Element::Model, MatId>());

    virtual void reassign( Element::Group<const Elem>& );

    void printInfo() const;

private:
	Coordinate::Group<CoordR3> coords_;
	Element::Group<ElemR> elems_;
	Layer::Group<Layer::Layer> layers_;
};

} /* namespace Mesh */
} /* namespace Geometry */
} /* namespace SEMBA */

