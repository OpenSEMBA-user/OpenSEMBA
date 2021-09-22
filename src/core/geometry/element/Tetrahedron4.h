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

#pragma once

#include "Tetrahedron.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Tetrahedron4 : public Tetrahedron {
public:
    static const std::size_t sizeOfCoordinates = 4;

    Tetrahedron4();
    Tetrahedron4(const Id id,
                 const CoordR3* v[4],
                 const Layer* lay = nullptr,
                 const Model* mat = nullptr);
    Tetrahedron4(const Tetrahedron4& rhs);
    virtual ~Tetrahedron4();

    SEMBA_CLASS_DEFINE_CLONE(Tetrahedron4);

    bool isInnerPoint(const Math::CVecR3& pos) const;
    bool isCurvedFace(const std::size_t face) const;
    bool isFaceContainedInPlane(
            const std::size_t face,
            const Math::Constants::CartesianPlane plane) const;

    std::size_t numberOfCoordinates() const { return sizeOfCoordinates; }

    std::size_t numberOfSideCoordinates(const std::size_t f = 0) const {
        return 3;
    }

    const CoordR3* getV(const std::size_t i) const { return v_[i]; }
    const CoordR3* getSideV(const std::size_t f, const std::size_t i) const;

    const CoordR3* getVertex(const std::size_t i) const {
        return v_[tet.vertex(i)];
    }
    const CoordR3* getSideVertex(const std::size_t f,
                                 const std::size_t i) const;

    Math::Real getVolume() const;
    const Math::Simplex::Simplex& getTet() const { return tet; }
    Math::Real getAreaOfFace(const std::size_t face) const;

    void setV(const std::size_t i, const CoordR3*);

    void printInfo() const;
    void check() const;

private:
    static const Math::Simplex::Tetrahedron<1> tet;
    const CoordR3* v_[4];

    bool hasZeroVolume() const;
};

} /* namespace Element */

typedef Element::Tetrahedron4 Tet4;

} /* namespace Geometry */
} /* namespace SEMBA */

