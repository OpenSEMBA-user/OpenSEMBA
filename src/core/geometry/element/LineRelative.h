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

#ifndef SEMBA_GEOMETRY_ELEMENT_LINERELATIVE_H_
#define SEMBA_GEOMETRY_ELEMENT_LINERELATIVE_H_

#include "geometry/coordinate/Relative.h"

#include "Line2.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class LineRelative : public virtual Line2<Math::Int> {
public:
    LineRelative();
    LineRelative(const Id id,
                 const CoordI3* v[2],
                 const Layer* lay = NULL,
                 const Model* mat = NULL);
    LineRelative(const CoordI3* v[2],
                 const Layer* lay = NULL,
                 const Model* mat = NULL);
    LineRelative(const LineRelative& rhs);
    virtual ~LineRelative();

    SEMBA_CLASS_DEFINE_CLONE(LineRelative);

    ElemR* toUnstructured(const Coordinate::Group<CoordR3>&,
                          const Grid3&) const;

    void printInfo() const;
};

namespace Error {
namespace Coord {

class NotRel : public Error {
public:
    NotRel(const CoordId& coordId)
    :   Error(coordId) {
        std::stringstream aux;
        aux << "Coordinate with Id (" << this->getCoordId()
            << ") not relative";
        str_ = aux.str();
    }
    ~NotRel() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

} /* namespace Coord */
} /* namespace Error */
} /* namespace Element */

typedef Element::LineRelative LinRel;

} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_ELEMENT_LINERELATIVE_H_ */
