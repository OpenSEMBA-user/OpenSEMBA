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

#include "Source.h"

#include "group/Group.h"
#include "group/Cloneable.h"
#include "group/Printable.h"

namespace SEMBA {
namespace Source {

template<typename S = Base>
class Group : public SEMBA::Group::Group<S>,
              public SEMBA::Group::Cloneable<S>,
              public SEMBA::Group::Printable<S> {
public:
    Group() {}
    template<typename S2>
    Group(S2* e)                     : SEMBA::Group::Group<S>(e) {}
    template<typename S2>
    Group(const std::vector<S2*>& e) : SEMBA::Group::Group<S>(e) {}
    template<typename S2>
    Group(SEMBA::Group::Group<S2>&       rhs) : SEMBA::Group::Group<S>(rhs) {}
    template<typename S2>
    Group(const SEMBA::Group::Group<S2>& rhs) : SEMBA::Group::Group<S>(rhs) {}
    Group(SEMBA::Group::Group<S>&        rhs) : SEMBA::Group::Group<S>(rhs) {}
    template<typename S2>
    Group(SEMBA::Group::Group<S2>&& rhs)
    :   SEMBA::Group::Group<S>(std::move(rhs)) {}
    Group(SEMBA::Group::Group<S >&& rhs)
    :   SEMBA::Group::Group<S>(std::move(rhs)) {}
    virtual ~Group() {}

    SEMBA_GROUP_DEFINE_CLONE(Group, S);

    Group& operator=(SEMBA::Group::Group<S>&);
    Group& operator=(SEMBA::Group::Group<S>&&);

    void printInfo() const;
};

} /* namespace Source */
} /* namespace SEMBA */

#include "Group.hpp"

namespace SEMBA {

typedef Source::Group<> SourceGroup;

} /* namespace SEMBA */

