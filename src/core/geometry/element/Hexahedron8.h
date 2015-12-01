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

#ifndef SEMBA_GEOMETRY_ELEMENT_HEXAHEDRON8_H_
#define SEMBA_GEOMETRY_ELEMENT_HEXAHEDRON8_H_

#include "Volume.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Hexahedron8Base : public virtual VolumeBase {
public:
    Hexahedron8Base() {}
    virtual ~Hexahedron8Base() {}

    inline bool isQuadratic() const { return false; }

    inline Size numberOfFaces      () const { return 6; }
    inline Size numberOfVertices   () const { return 8; }
    inline Size numberOfCoordinates() const { return 8; }

    inline Size numberOfSideVertices   (const Size f = 0) const { return 4; }
    inline Size numberOfSideCoordinates(const Size f = 0) const { return 4; }
};

template<class T>
class Hexahedron8 : public virtual Volume<T>,
              public virtual Hexahedron8Base {
public:
    Hexahedron8();
    Hexahedron8(const Id id,
                const Coordinate::Coordinate<T,3>* v[8],
                const Layer* lay = NULL,
                const Model* mat = NULL);
    Hexahedron8(Coordinate::Group<Coordinate::Coordinate<T,3> >&,
                const Id id,
                const Box<T,3>& box,
                const Layer* lay = NULL,
                const Model* mat = NULL);
    Hexahedron8(const Hexahedron8<T>& rhs);
    virtual ~Hexahedron8();

    SEMBA_CLASS_DEFINE_CLONE(Hexahedron8<T>);

    bool isStructured(const Grid3&, const Math::Real = Grid3::tolerance) const;

    // Checks if edges are ortonormal.
    bool isRegular() const;
    inline bool isCurvedFace(const Size f) const { return false; }

    const Coordinate::Coordinate<T,3>* getV    (const Size i) const;
    const Coordinate::Coordinate<T,3>* getSideV(const Size f,
                                                const Size i) const;

    const Coordinate::Coordinate<T,3>* getVertex    (const Size i) const;
    const Coordinate::Coordinate<T,3>* getSideVertex(const Size f,
                                                     const Size i) const;

    std::vector<const Coordinate::Coordinate<T,3>*> getVertices() const;
    std::vector<const Coordinate::Coordinate<T,3>*> getSideVertices(
            const Size face) const;

    Math::Real getAreaOfFace(const Size face) const;
    Math::Real getVolume() const;

    void setV(const Size i, const Coordinate::Coordinate<T,3>*);

    ElemI* toStructured(const Coordinate::Group<CoordI3>&,
                        const Grid3&,
                        const Math::Real = Grid3::tolerance) const;
    ElemR* toUnstructured(const Coordinate::Group<CoordR3>&,
                          const Grid3&) const;

    void printInfo() const;

private:
    const Coordinate::Coordinate<T,3>* v_[8];

    const static Math::Real tolerance;
};

} /* namespace Element */

typedef Element::Hexahedron8Base         Hex8;
typedef Element::Hexahedron8<Math::Real> HexR8;
typedef Element::Hexahedron8<Math::Int > HexI8;

} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_ELEMENT_HEXAHEDRON8_H_ */
