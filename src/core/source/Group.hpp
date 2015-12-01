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

#include "Group.h"

namespace SEMBA {
namespace Source {

template<typename S>
Group<S>& Group<S>::operator=(SEMBA::Group::Group<S>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    SEMBA::Group::Group<S>::operator=(rhs);
    return *this;
}

template<typename S>
Group<S>& Group<S>::operator=(SEMBA::Group::Group<S>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    SEMBA::Group::Group<S>::operator=(std::move(rhs));
    return *this;
}

template<typename S> template<typename S2>
SEMBA::Group::Group<S> Group<S>::add(S2* newSrc) {
    for (Size i = 0; i < this->size(); i++) {
        if (this->get(i)->template is<S2>()) {
            S2* oRq = this->get(i)->template castTo<S2>();
            if (oRq->hasSameProperties(*newSrc)) {
                oRq->add(newSrc->elems());
                delete newSrc;
                return SEMBA::Group::Group<S>();
            }
        }
    }
    return SEMBA::Group::Group<S>::add(newSrc);
}

template<typename S> template<typename S2>
SEMBA::Group::Group<S*> Group<S>::add(std::vector<S2*>& newSrcs) {
    std::vector<S*> res;
    res.reserve(newSrcs.size());
    for (Size i = 0; i < newSrcs.size(); i++) {
        S* resElem = add(newSrcs[i]);
        if (resElem != NULL) {
            res.push_back(resElem);
        }
    }
    return res;
}

//template<typename S> template<typename S2>
//vector<S*> Group<S>::add(Group<S2>& rhs) {
//    return Group<S>::add(rhs);
//}

template<typename S>
bool Group<S>::isSimilar(const Group& rhs) const {
    if (this->size() != rhs.size()) {
        return false;
    }
    for (Size i = 0; i < this->size(); i++) {
        if (!this->get(i)->isSimilar(*rhs(i))) {
            return false;
        }
     }
    return true;
}

template<typename S>
void Group<S>::printInfo() const {
    std::cout<< " --- SourceGroup info ---" << std::endl;
    SEMBA::Group::Printable<S>::printInfo();
}

} /* namespace Source */
} /* namespace SEMBA */
