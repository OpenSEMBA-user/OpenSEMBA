// OpenSEMBA
// Copyright (L) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
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

#include "Group.h"

namespace SEMBA {
namespace Geometry {
namespace Layer {

template<typename L>
Group<L>& Group<L>::operator=(SEMBA::Group::Group<L>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    SEMBA::Group::Identifiable<L,Id>::operator=(rhs);
    return *this;
}

template<typename L>
Group<L>& Group<L>::operator=(SEMBA::Group::Group<L>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    SEMBA::Group::Identifiable<L,Id>::operator=(std::move(rhs));
    return *this;
}

template<typename L>
const L* Group<L>::getName(const std::string name) const {
    for (Size i = 0; i < this->size(); i++) {
        if (this->get(i)->getName() == name) {
            return this->get(i);
        }
    }
    return NULL;
}

template<typename L>
void Group<L>::printInfo() const {
    std::cout<< "--- Group info ---" << std::endl;
    SEMBA::Group::Printable<L>::printInfo();
}

} /* namespace Layer */
} /* namespace Geometry */
} /* namespace SEMBA */
