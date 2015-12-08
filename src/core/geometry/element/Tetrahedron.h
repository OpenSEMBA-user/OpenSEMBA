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

#ifndef SEMBA_GEOMETRY_ELEMENT_TETRAHEDRON_H_
#define SEMBA_GEOMETRY_ELEMENT_TETRAHEDRON_H_

#include "math/simplex/Tetrahedron.h"
#include "Triangle6.h"

#include "Volume.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Tetrahedron : public Volume<Math::Real> {
public:
    Tetrahedron();
    virtual ~Tetrahedron();

    virtual bool isCurvedFace(const std::size_t face) const = 0;
    virtual bool isFaceContainedInPlane(const std::size_t face,
            const Math::Constants::CartesianPlane plane) const = 0;

    inline std::size_t numberOfFaces   () const { return 4; }
    inline std::size_t numberOfVertices() const { return 4; }
    inline std::size_t numberOfSideVertices(const std::size_t f = 0) const {
        return 3;
    }
    virtual const Math::Simplex::Simplex& getTet() const = 0;
    virtual Math::Real getVolume() const = 0;
    virtual Math::Real getAreaOfFace(const std::size_t face) const = 0;
    virtual Triangle3* getTri3Face(const std::size_t f) const;
    void getCubaturePositions(
            Math::CVecR3 cubaturePositions[
                            Math::Simplex::Tetrahedron<1>::ncp]) const;
    void getCubatureJacobian(
            Math::MatR44 cJ[Math::Simplex::Tetrahedron<2>::ncp]) const;
    void getCubatureJacobianDeterminant(
            Math::Real cJDet[Math::Simplex::Tetrahedron<2>::ncp],
            const Math::MatR44 cJ[Math::Simplex::Tetrahedron<2>::ncp]) const;
    void getCubatureJacobianDeterminant(
            Math::Real cJDet[Math::Simplex::Tetrahedron<2>::ncp]) const;
    void getCubatureJacobianHat(
            Math::MatR43 cJHat[Math::Simplex::Tetrahedron<2>::ncp],
            const Math::MatR44 cJ[Math::Simplex::Tetrahedron<2>::ncp],
            const Math::Real cJDet[Math::Simplex::Tetrahedron<2>::ncp]) const;
};

} /* namespace Element */

typedef Element::Tetrahedron Tet;

} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_ELEMENT_TETRAHEDRON_H_ */
