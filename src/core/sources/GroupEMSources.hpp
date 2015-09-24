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
 * GroupEMSources.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */
#include "../sources/GroupEMSources.h"

template<typename E>
GroupEMSources<E>& GroupEMSources<E>::operator=(VectorPtr<E>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    Group<E>::operator=(rhs);
    return *this;
}

template<typename E>
GroupEMSources<E>& GroupEMSources<E>::operator=(VectorPtr<E>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    Group<E>::operator=(std::move(rhs));
    return *this;
}

template<typename E>
bool GroupEMSources<E>::isSimilar(const GroupEMSources& rhs) const {
    if (this->size() != rhs.size()) {
        return false;
    }
    for (UInt i = 0; i < this->size(); i++) {
        if (!this->get(i)->isSimilar(*rhs(i))) {
            return false;
        }
    }
    return true;
}

template<typename E>
bool GroupEMSources<E>::check() const {
    for (UInt i = 0; i < this->size(); i++) {
        if (!this->get(i)->check()) {
            return false;
        }
    }
    return true;
}

template<typename E>
void GroupEMSources<E>::printInfo() const {
    cout << " --- EMSourceGroup info --- " << endl;
    Group<E>::printInfo();
}
