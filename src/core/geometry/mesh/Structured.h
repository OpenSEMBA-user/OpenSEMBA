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

#ifndef SEMBA_GEOMETRY_MESH_STRUCTURED_H_
#define SEMBA_GEOMETRY_MESH_STRUCTURED_H_

#include <exception>

#include "geometry/element/Hexahedron8.h"

#include "Mesh.h"
#include "geometry/Grid.h"
#include "geometry/coordinate/Group.h"
#include "geometry/element/Group.h"
#include "geometry/layer/Group.h"

class Unstructured;

namespace SEMBA {
namespace Geometry {
namespace Mesh {

class Structured : public virtual Mesh,
                   public virtual Grid3,
                   public virtual Coordinate::Group<CoordI3>,
                   public virtual Element::Group<ElemI>,
                   public virtual Layer::Group<Layer::Layer> {
public:
    typedef std::array<std::array<const Element::Model*,2>,3>
                BoundTerminations;

    Structured(const Grid3& grid);
    Structured(const Grid3& grid,
               const Coordinate::Group<const CoordI3>& cG,
               const Element::Group<const ElemI>& elem,
               const Layer::Group<const Layer::Layer>& =
                   Layer::Group<const Layer::Layer>(),
               const BoundTerminations& =
                   {{{NULL,NULL},{NULL,NULL},{NULL,NULL}}});
    Structured(const Structured& param);
    virtual ~Structured();

    Structured& operator=(const Structured& rhs);

    SEMBA_CLASS_DEFINE_CLONE(Structured);

    BoundTerminations&          bounds() { return bounds_; }
    Grid3&                      grid  () { return *this;   }
    Coordinate::Group<CoordI3>& coords() { return *this;   }
    Element::Group<ElemI>&      elems () { return *this;   }
    Layer::Group<>&             layers() { return *this;   }

    const BoundTerminations&          bounds() const { return bounds_; }
    const Grid3&                      grid  () const { return *this;   }
    const Coordinate::Group<CoordI3>& coords() const { return *this;   }
    const Element::Group<ElemI>&      elems () const { return *this;   }
    const Layer::Group<>&             layers() const { return *this;   }

    Unstructured* getMeshUnstructured() const;
    Structured* getConnectivityMesh() const;

    template<template<typename> class E>
    Element::Group< E<Math::Int> > add(
            const Element::Group<E<Math::Real> >&,
            const Math::Real tol = Grid3::tolerance);
    template<template<typename> class E>
    Element::Group< E<Math::Int> > add(
            const Element::Group<const E<Math::Real> >&,
            const Math::Real tol = Grid3::tolerance);
    template<template<typename> class E>
    Element::Group< E<Math::Int> > add(
            E<Math::Real>*,
            const Math::Real tol = Grid3::tolerance);

    void convertToHex(Element::Group<const SurfI> surfs);
    void addAsHex(Element::Group<const VolR> vols);

    Math::Real getMinimumSpaceStep() const;
    void applyScalingFactor(const Math::Real factor);
    BoxR3 getBoundingBox() const;

    virtual void printInfo() const;
private:
    BoundTerminations bounds_;

    std::vector<HexI8*> discretizeWithinBoundary(
            const Element::Group<const SurfI>& faces);
    std::vector<std::pair<const SurfI*, const SurfI*> >
        getPairsDefiningVolumeWithin(
            const Element::Group<const SurfI>& faces) const;
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
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_MESH_STRUCTURED_H_ */
