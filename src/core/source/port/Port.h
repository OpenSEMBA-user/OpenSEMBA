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

#ifndef SEMBA_SOURCE_PORT_PORT_H_
#define SEMBA_SOURCE_PORT_PORT_H_

#include "../Source.h"

namespace SEMBA {
namespace Source {
namespace Port {

class Port : public Source<Geometry::Surf> {
public:

    Port(Magnitude::Magnitude* magnitude,
         const Geometry::Element::Group<const Geometry::Surf>& elem);
    Port(const Port& rhs);
    virtual ~Port();

    Math::CVecR3 getNormal() const;

//    virtual Math::CVecR3 getOrigin(const BoundTerminations& sym) const = 0;
//    virtual Math::CVecR3 getWeight(
//            const Math::CVecR3& pos,
//            const BoundTerminations& sym) const = 0;
};

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */

#endif /* SEMBA_SOURCE_PORT_PORT_H_ */
