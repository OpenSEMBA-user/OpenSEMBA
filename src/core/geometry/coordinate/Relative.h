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

#ifndef SEMBA_GEOMETRY_COORDINATE_RELATIVE_H_
#define SEMBA_GEOMETRY_COORDINATE_RELATIVE_H_

#include "Coordinate.h"

namespace SEMBA {
namespace Geometry {
namespace Coordinate {

class Relative : public virtual Coordinate<Math::Int,3> {
public:
    Relative();
    Relative(const Id,
             const Math::CVecR3&);
    Relative(const Id,
             const Math::CVecI3&,
             const Math::CVecR3&);
    Relative(const Relative&);
    virtual ~Relative();

    SEMBA_CLASS_DEFINE_CLONE(Relative);

    Relative& operator=(const Relative& rhs);

    bool operator==(const Base& rhs) const;

    Math::CVecR3&       rel()       { return rel_; }
    const Math::CVecR3& rel() const { return rel_; }

    CoordR3* toUnstructured(const Grid3&) const;

    void printInfo() const;

private:
    Math::CVecR3 rel_;
};

} /* namespace Coordinate */

typedef Coordinate::Relative CoordRel;

} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_COORDINATE_RELATIVE_H_ */
