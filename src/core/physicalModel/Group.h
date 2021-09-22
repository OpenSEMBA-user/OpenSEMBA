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

#pragma once

#include "PhysicalModel.h"

#include "group/Cloneable.h"
#include "group/Printable.h"
#include "group/Identifiable.h"

namespace SEMBA {
namespace PhysicalModel {

template<typename P = PhysicalModel>
class Group : public SEMBA::Group::Cloneable<P>,
              public SEMBA::Group::Printable<P>,
              public SEMBA::Group::Identifiable<P, Id> {
public:
    Group() {}
    template<typename P2>
    Group(P2* e)                     : SEMBA::Group::Identifiable<P,Id>(e) {}
    template<typename P2>
    Group(const std::vector<P2*>& e) : SEMBA::Group::Identifiable<P,Id>(e) {}
    template<typename P2>
    Group(SEMBA::Group::Group<P2>& rhs)
    :   SEMBA::Group::Identifiable<P,Id>(rhs) {}
    template<typename P2>
    Group(const SEMBA::Group::Group<P2>& rhs)
    :   SEMBA::Group::Identifiable<P,Id>(rhs) {}
    Group(SEMBA::Group::Group<P>& rhs)
    :   SEMBA::Group::Identifiable<P,Id>(rhs) {}
    template<typename P2>
    Group(SEMBA::Group::Group<P2>&& rhs)
    :   SEMBA::Group::Identifiable<P,Id>(std::move(rhs)) {}
    Group(SEMBA::Group::Group<P >&& rhs)
    :   SEMBA::Group::Identifiable<P,Id>(std::move(rhs)) {}
    virtual ~Group() {}

    SEMBA_GROUP_DEFINE_CLONE(Group, P);

    Group& operator=(SEMBA::Group::Group<P>&);
    Group& operator=(SEMBA::Group::Group<P>&&);

    void printInfo() const;
};

} /* namespace PhysicalModel */

typedef PhysicalModel::Group<> PMGroup;

} /* namespace SEMBA */

#include "Group.hpp"

