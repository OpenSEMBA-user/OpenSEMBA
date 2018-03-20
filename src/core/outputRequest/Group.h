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

#ifndef SEMBA_OUTPUTREQUEST_GROUP_H_
#define SEMBA_OUTPUTREQUEST_GROUP_H_

#include "OutputRequest.h"

#include "group/Group.h"
#include "group/Cloneable.h"
#include "group/Printable.h"

namespace SEMBA {
namespace OutputRequest {

template<typename O = Base>
class Group : public SEMBA::Group::Group<O>,
              public SEMBA::Group::Cloneable<O>,
              public SEMBA::Group::Printable<O> {
public:
    Group() {}
    template<typename O2>
    Group(O2* e)                     : SEMBA::Group::Group<O>(e) {}
    template<typename O2>
    Group(const std::vector<O2*>& e) : SEMBA::Group::Group<O>(e) {}
    template<typename O2>
    Group(SEMBA::Group::Group<O2>&       rhs) : SEMBA::Group::Group<O>(rhs) {}
    template<typename O2>
    Group(const SEMBA::Group::Group<O2>& rhs) : SEMBA::Group::Group<O>(rhs) {}
    Group(SEMBA::Group::Group<O>&        rhs) : SEMBA::Group::Group<O>(rhs) {}
    template<typename O2>
    Group(SEMBA::Group::Group<O2>&& rhs)
    :   SEMBA::Group::Group<O>(std::move(rhs)) {}
    Group(SEMBA::Group::Group<O >&& rhs)
    :   SEMBA::Group::Group<O>(std::move(rhs)) {}
    virtual ~Group() {}

    SEMBA_GROUP_DEFINE_CLONE(Group, O);

    Group& operator=(SEMBA::Group::Group<O>&);
    Group& operator=(SEMBA::Group::Group<O>&&);

    bool isSimilar(const Group& rhs) const;

    void printInfo() const;
};

} /* namespace OutputRequest */
} /* namespace SEMBA */

#include "Group.hpp"

#endif /* SEMBA_OUTPUTREQUEST_GROUP_H_ */
