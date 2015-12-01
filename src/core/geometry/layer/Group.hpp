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
/*
 * GroupLayers.cpp
 *
 *  Created on: Dec 3, 2014
 *      Author: luis
 */

#include "GroupLayers.h"

template<typename L>
GroupLayers<L>& GroupLayers<L>::operator=(VectorPtr<L>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    GroupId<L, LayerId>::operator=(rhs);
    return *this;
}

template<typename L>
GroupLayers<L>& GroupLayers<L>::operator=(VectorPtr<L>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    GroupId<L, LayerId>::operator=(std::move(rhs));
    return *this;
}

template<typename L>
const L* GroupLayers<L>::getName(const string name) const {
    for (UInt i = 0; i < this->size(); i++) {
        if (this->get(i)->getName() == name) {
            return this->get(i);
        }
    }
    return NULL;
}

template<typename L>
void GroupLayers<L>::printInfo() const {
    cout<< "--- GroupLayers info ---" << endl;
    GroupId<L, LayerId>::printInfo();
}
