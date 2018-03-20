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

#ifndef SEMBA_GEOMETRY_ELEMENT_TRIANGLE6_H_
#define SEMBA_GEOMETRY_ELEMENT_TRIANGLE6_H_

#include "Triangle.h"
#include "Triangle3.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Triangle6 : public Triangle {
public:
    Triangle6();
    Triangle6(const Id id,
              const CoordR3*[6],
              const Layer* lay = nullptr,
              const Model* mat = nullptr);
    Triangle6(const Triangle6& rhs);
    virtual ~Triangle6();

    SEMBA_CLASS_DEFINE_CLONE(Triangle6);

    bool isCurved   () const;
    bool isQuadratic() const { return true; }

    std::size_t numberOfCoordinates() const { return 6; }

    std::size_t numberOfSideCoordinates(const std::size_t f = 0) const {
        return 3;
    }

    const CoordR3* getV(const std::size_t i) const {return v_[i];}
    const CoordR3* getSideV(const std::size_t face, const std::size_t i) const;

    const CoordR3* getVertex(const std::size_t i) const;
    const CoordR3* getSideVertex(const std::size_t face,
                                 const std::size_t i) const;

    Math::Real getArea() const;

    void setV(const std::size_t i, const CoordR3*);

    Triangle3* linearize() const;

    void printInfo() const;

protected:
    static const Math::Simplex::Triangle<2> geo;
    const CoordR3* v_[6];
};

} /* namespace Element */

typedef Element::Triangle6 Tri6;

} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_ELEMENT_TRIANGLE6_H_ */
