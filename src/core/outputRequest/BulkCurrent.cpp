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

#include "BulkCurrent.h"

namespace SEMBA {
namespace OutputRequest {

BulkCurrent::BulkCurrent(
        const Domain& domain,
        const std::string& name,
        const Geometry::Element::Group<const Geometry::Elem>& elem,
        const Math::Constants::CartesianAxis& dir,
        const Math::UInt& skip)
:   SEMBA::OutputRequest::Base(bulkCurrentElectric, name, domain),
    Geometry::Element::Group<const Geometry::Elem>(elem) {

    dir_ = dir;
    skip_ = skip;
}

BulkCurrent::BulkCurrent(const BulkCurrent& rhs)
:   SEMBA::OutputRequest::Base(rhs),
    Geometry::Element::Group<const Geometry::Elem>(rhs) {

    dir_ = rhs.dir_;
    skip_ = rhs.skip_;
}

BulkCurrent::~BulkCurrent() {

}

Math::Constants::CartesianAxis BulkCurrent::getDir() const {
    return dir_;
}

Math::UInt BulkCurrent::getSkip() const {
    return skip_;
}

} /* namespace OutputRequest */
} /* namespace SEMBA */
