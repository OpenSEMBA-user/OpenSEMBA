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

#include "Port.h"

namespace SEMBA {
namespace Source {
namespace Port {

Port::Port(Magnitude::Magnitude* magnitude,
           const Geometry::Element::Group<const Geometry::Surf>& elem)
:   SEMBA::Source::Base(magnitude),
    Geometry::Element::Group<const Geometry::Surf>(elem) {
}

Port::Port(const Port& rhs) :
            SEMBA::Source::Base(rhs),
            Geometry::Element::Group<const Geometry::Surf>(rhs) {
}

Port::~Port() {
}

Math::CVecR3 Port::getNormal() const {
    if (this->getOf<Geometry::Surf>().size() > 0) {
        if (this->get(0)->is<Geometry::SurfR>()) {
            return this->get(0)->castTo<Geometry::SurfR>()->getNormal();
        } else {
            Math::CVecI3 aux =
                this->get(0)->castTo<Geometry::SurfI>()->getNormal();
            Math::CVecR3 res;
            for (Math::UInt d = 0; d < 3; d++) {
                res(d) = (Math::Real) aux(d);
            }
            return res;
        }
    }
    return Math::CVecR3();
}

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */

