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
namespace Argument {

template<bool S>
Group<S>::Group(Group<true>* group,
                const std::string& name,
                const std::string& desc)
:   GroupBase(group, name, desc) {
    numMutExc_ = 0;
}

template<bool S>
Group<S>::~Group() {

}

template<bool S>
bool Group<S>::isRequired() const {
    if (isMutuallyExclusive()) {
        return GroupBase::isRequired();
    }
    return false;
}

template<bool S>
Group<S>& Group<S>::required() {
    if (isMutuallyExclusive()) {
        GroupBase::required();
    }
    return *this;
}

template<bool S> template<bool S2, typename T>
Group<true>& Group<S>::addGroup(const std::string& name,
                                const std::string& desc) {
    insertName_(name);
    Group<true>* newGroup = new Group<true>(this, name, desc);

    childName_[name] = child_.size();
    child_.push_back(newGroup);

    return *newGroup;
}

template<bool S> template<bool S2, typename T>
Group<false>& Group<S>::addMutuallyExclusiveGroup() {
    std::stringstream aux;
    aux << "__aux" << ++numMutExc_ << "__";
    std::string name = aux.str();
    insertName_(aux.str());
    Group<false>* newGroup = new Group<false>(this, name);

    childName_[name] = child_.size();
    child_.push_back(newGroup);

    return *newGroup;
}

} /* namespace Argument */
} /* namespace SEMBA */
