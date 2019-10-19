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

#ifndef SEMBA_GEOMETRY_LAYER_GROUP_H_
#define SEMBA_GEOMETRY_LAYER_GROUP_H_

#include "Layer.h"
#include "group/Cloneable.h"
#include "group/Printable.h"
#include "group/Identifiable.h"

namespace SEMBA {
namespace Geometry {
namespace Layer {

template<typename L = Layer>
class Group : public SEMBA::Group::Cloneable<L>,
              public SEMBA::Group::Printable<L>,
              public SEMBA::Group::Identifiable<L, Id>  {
public:
    Group() {}
    template<typename L2>
    Group(L2* e)                     : SEMBA::Group::Identifiable<L,Id>(e) {}
    template<typename L2>
    Group(const std::vector<L2*>& e) : SEMBA::Group::Identifiable<L,Id>(e) {}
    template<typename L2>
    Group(SEMBA::Group::Group<L2>& rhs)
    :   SEMBA::Group::Identifiable<L,Id>(rhs) {}
    template<typename L2>
    Group(const SEMBA::Group::Group<L2>& rhs)
    :   SEMBA::Group::Identifiable<L,Id>(rhs) {}
    Group(SEMBA::Group::Group<L>& rhs)
    :   SEMBA::Group::Identifiable<L,Id>(rhs) {}
    template<typename L2>
    Group(SEMBA::Group::Group<L2>&& rhs)
    :   SEMBA::Group::Identifiable<L,Id>(std::move(rhs)) {}
    Group(SEMBA::Group::Group<L >&& rhs)
    :   SEMBA::Group::Identifiable<L,Id>(std::move(rhs)) {}
    virtual ~Group() {}

    SEMBA_GROUP_DEFINE_CLONE(Group, L);

    Group& operator=(SEMBA::Group::Group<L>&);
    Group& operator=(SEMBA::Group::Group<L>&&);

    const L* getName(const std::string name) const;

    void printInfo() const;
};

} /* namespace Layer */

typedef Layer::Group<> LayerGroup;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Group.hpp"

#endif /* SEMBA_GEOMETRY_LAYER_GROUP_H_ */
