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
#include "GroupOutRqs.h"

template<typename O>
GroupOutRqs<O>& GroupOutRqs<O>::operator=(VectorPtr<O>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    Group<O>::operator=(rhs);
    return *this;
}

template<typename O>
GroupOutRqs<O>& GroupOutRqs<O>::operator=(VectorPtr<O>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    Group<O>::operator=(std::move(rhs));
    return *this;
}

template<typename O> template<typename O2>
VectorPtr<O> GroupOutRqs<O>::add(O2* newORq) {
    for (UInt i = 0; i < this->size(); i++) {
        if (this->get(i)->template is<O2>()) {
            O2* oRq = this->get(i)->template castTo<O2>();
            if (oRq->hasSameProperties(*newORq)) {
                oRq->add(newORq->elems());
                delete newORq;
                return VectorPtr<O>();
            }
        }
    }
    return Group<O>::add(newORq);
}

template<typename O> template<typename O2>
vector<O*> GroupOutRqs<O>::add(vector<O2*>& newOuts) {
    vector<O*> res;
    res.reserve(newOuts.size());
    for (UInt i = 0; i < newOuts.size(); i++) {
        O* resElem = add(newOuts[i]);
        if (resElem != NULL) {
            res.push_back(resElem);
        }
    }
    return res;
}

//template<typename O> template<typename O2>
//vector<O*> GroupOutRqs<O>::add(Group<O2>& rhs) {
//    return Group<O>::add(rhs);
//}

template<typename O>
bool GroupOutRqs<O>::isSimilar(const GroupOutRqs& rhs) const {
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

template<typename O>
void GroupOutRqs<O>::printInfo() const {
    cout<< " --- OutputRequestGroup info ---" << endl;
    Group<O>::printInfo();
}
