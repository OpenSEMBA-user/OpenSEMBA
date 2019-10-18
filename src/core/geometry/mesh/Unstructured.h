// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#ifndef SEMBA_GEOMETRY_MESH_UNSTRUCTURED_H_
#define SEMBA_GEOMETRY_MESH_UNSTRUCTURED_H_

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

#endif /* SEMBA_GEOMETRY_MESH_UNSTRUCTURED_H_ */
