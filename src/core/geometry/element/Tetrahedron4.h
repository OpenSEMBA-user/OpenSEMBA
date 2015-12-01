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

#ifndef SEMBA_GEOMETRY_ELEMENT_TETRAHEDRON4_H_
#define SEMBA_GEOMETRY_ELEMENT_TETRAHEDRON4_H_

#include "Tetrahedron.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Tetrahedron4 : public Tetrahedron {
public:
    Tetrahedron4();
    Tetrahedron4(const Id id,
                 const CoordR3* v[4],
                 const Layer* lay = NULL,
                 const Model* mat = NULL);
    Tetrahedron4(const Tetrahedron4& rhs);
    virtual ~Tetrahedron4();

    SEMBA_CLASS_DEFINE_CLONE(Tetrahedron4);

    bool isInnerPoint(const Math::CVecR3& pos) const;
    bool isCurvedFace(const Size face) const;
    bool isFaceContainedInPlane(
            const Size face,
            const Math::Constants::CartesianPlane plane) const;

    Size numberOfCoordinates() const { return 4; }

    Size numberOfSideCoordinates(const Size f = 0) const { return 3; }

    const CoordR3* getV(const Size i) const { return v_[i]; }
    const CoordR3* getSideV(const Size f, const Size i) const;

    const CoordR3* getVertex(const Size i) const { return v_[tet.vertex(i)]; }
    const CoordR3* getSideVertex(const Size f, const Size i) const;

    Math::Real getVolume() const;
    const Math::Simplex::Simplex& getTet() const { return tet; }
    Math::Real getAreaOfFace(const Size face) const;

    void setV(const Size i, const CoordR3*);

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

#endif /* SEMBA_GEOMETRY_ELEMENT_TETRAHEDRON4_H_ */
