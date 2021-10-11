

#pragma once

#include "PhysicalModel.h"

#include "group/Cloneable.h"
#include "group/Identifiable.h"

namespace SEMBA {
namespace PhysicalModel {

template<typename P = PhysicalModel>
class Group : public SEMBA::Group::Cloneable<P>,
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

};

} /* namespace PhysicalModel */

typedef PhysicalModel::Group<> PMGroup;

} /* namespace SEMBA */

#include "Group.hpp"

