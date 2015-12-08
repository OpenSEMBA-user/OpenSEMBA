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

#ifndef SEMBA_GEOMETRY_ELEMENT_TETRAHEDRON10_H_
#define SEMBA_GEOMETRY_ELEMENT_TETRAHEDRON10_H_

#include "Tetrahedron.h"
#include "Tetrahedron4.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Tetrahedron10 : public Tetrahedron {
public:
    Tetrahedron10();
    Tetrahedron10(const Id id,
                  const CoordR3* v[10],
                  const Layer* lay = NULL,
                  const Model* mat = NULL);
    Tetrahedron10(const Tetrahedron10& rhs);
    virtual ~Tetrahedron10();

    SEMBA_CLASS_DEFINE_CLONE(Tetrahedron10);

    bool isCurved() const;
    bool isQuadratic() const {return true;}
    bool isCurvedFace(const std::size_t f) const;
    bool isFaceContainedInPlane(
            const std::size_t face,
            const Math::Constants::CartesianPlane plane) const;

    std::size_t numberOfCoordinates() const { return 10; }

    std::size_t numberOfSideCoordinates(const std::size_t f = 0) const {
        return 6;
    }

    const CoordR3* getV(const std::size_t i) const {return v_[i];}
    const CoordR3* getSideV(const std::size_t f, const std::size_t i) const;

    const CoordR3* getVertex(const std::size_t i) const;
    const CoordR3* getSideVertex(const std::size_t f,
                                 const std::size_t i) const;

    Math::Real getVolume() const;
    const Math::Simplex::Simplex& getTet() const {return tet;}
    Math::Real getAreaOfFace(const std::size_t face) const;
    Triangle6 getTri6Face(const std::size_t f) const;

    void setV(const std::size_t i, const CoordR3*);

    Tetrahedron4* linearize() const;

    void printInfo() const;
    void check() const;

private:
    static const Math::Simplex::Triangle<2> tri;
    static const Math::Simplex::Tetrahedron<2> tet;
    const CoordR3* v_[10];
};

} /* namespace Element */

typedef Element::Tetrahedron10 Tet10;

} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_ELEMENT_TETRAHEDRON10_H_ */
