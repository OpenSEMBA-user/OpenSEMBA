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

#ifndef SEMBA_GEOMETRY_ELEMENT_LINECONFORMAL_H_
#define SEMBA_GEOMETRY_ELEMENT_LINECONFORMAL_H_

#include "geometry/coordinate/Conformal.h"

#include "Line2.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class LineConformal : public virtual Line2<Math::Int> {
public:
    LineConformal();
    LineConformal(const Id id,
                  const CoordI3* v[2],
                  const Math::CVecR3& norm,
                  const Layer* lay = NULL,
                  const Model* mat = NULL);
    LineConformal(const CoordI3* v[2],
                  const Math::CVecR3& norm,
                  const Layer* lay = NULL,
                  const Model* mat = NULL);
    LineConformal(const LineConformal& rhs);
    virtual ~LineConformal();

    SEMBA_CLASS_DEFINE_CLONE(LineConformal);

    Math::CVecR3 getNorm () const { return norm_;  }

    const CoordConf* getV(const Size i) const;

    void setV(const Size i, const CoordI3* coord);

    ElemR* toUnstructured(const Coordinate::Group<CoordR3>&,
                          const Grid3&) const;

    void printInfo() const;

private:
    void checkCoordinates();
    Math::CVecR3 norm_;
};

namespace Error {
namespace Coord {

class NotConf : public Error {
public:
    NotConf(const CoordId& coordId)
    :   Error(coordId) {
        std::stringstream aux;
        aux << "Coordinate with Id (" << this->getCoordId()
            << ") not conformal";
        str_ = aux.str();
    }
    ~NotConf() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

} /* namespace Coord */
} /* namespace Error */
} /* namespace Element */

typedef Element::LineConformal LinConf;

} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_ELEMENT_LINECONFORMAL_H_ */
