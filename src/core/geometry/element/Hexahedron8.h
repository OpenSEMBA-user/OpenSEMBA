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

#include <array>

#include "Volume.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Hexahedron8Base : public virtual VolumeBase {
public:
    static const std::size_t sizeOfCoordinates = 8;

    Hexahedron8Base() {}
    virtual ~Hexahedron8Base() {}

    inline bool isQuadratic() const { return false; }

    inline std::size_t numberOfFaces      () const { return 6; }
    inline std::size_t numberOfVertices   () const { return 8; }
    inline std::size_t numberOfCoordinates() const { return sizeOfCoordinates; }

    inline std::size_t numberOfSideVertices   (const std::size_t f = 0) const {
        return 4;
    }
    inline std::size_t numberOfSideCoordinates(const std::size_t f = 0) const {
        return 4;
    }
};

template<class T>
class Hexahedron8 : public virtual Volume<T>,
              public virtual Hexahedron8Base {
public:
    Hexahedron8();
    Hexahedron8(const Id id,
                const Coordinate::Coordinate<T,3>* v[8],
				const Layer* lay,
                const Model* mat = NULL);
    Hexahedron8(Coordinate::Group<Coordinate::Coordinate<T,3> >&,
                const Id id,
                const Box<T,3>& box,
                const Layer* lay = NULL,
                const Model* mat = NULL);
    Hexahedron8(const Hexahedron8<T>& rhs);
    virtual ~Hexahedron8();

    SEMBA_CLASS_DEFINE_CLONE(Hexahedron8);

    bool isStructured(const Grid3&, const Math::Real = Grid3::tolerance) const;

    // Checks if edges are ortonormal.
    bool isRegular() const;
    inline bool isCurvedFace(const std::size_t f) const { return false; }

    const Coordinate::Coordinate<T,3>* getV    (const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideV(const std::size_t f,
                                                const std::size_t i) const;

    const Coordinate::Coordinate<T,3>* getVertex    (
            const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideVertex(
            const std::size_t f,
            const std::size_t i) const;

    std::vector<const Coordinate::Coordinate<T,3>*> getVertices() const;
    std::vector<const Coordinate::Coordinate<T,3>*> getSideVertices(
            const std::size_t face) const;

    Math::Real getAreaOfFace(const std::size_t face) const;
    Math::Real getVolume() const;

    void setV(const std::size_t i, const Coordinate::Coordinate<T,3>*);

    ElemI* toStructured(const Coordinate::Group<CoordI3>&,
                        const Grid3&,
                        const Math::Real = Grid3::tolerance) const;
    ElemR* toUnstructured(const Coordinate::Group<CoordR3>&,
                          const Grid3&) const;

    void printInfo() const;

private:
    std::array<const Coordinate::Coordinate<T,3>*, 8> v_;

    const static Math::Real tolerance;
};

} /* namespace Element */

typedef Element::Hexahedron8Base         Hex8;
typedef Element::Hexahedron8<Math::Real> HexR8;
typedef Element::Hexahedron8<Math::Int > HexI8;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Hexahedron8.hpp"

#endif /* SEMBA_GEOMETRY_ELEMENT_HEXAHEDRON8_H_ */
