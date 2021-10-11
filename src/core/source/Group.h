

#pragma once

#include "Source.h"

#include "group/Group.h"
#include "group/Cloneable.h"

namespace SEMBA {
namespace Source {

template<typename S = Base>
class Group : public SEMBA::Group::Group<S>,
              public SEMBA::Group::Cloneable<S> {
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

};

} /* namespace Source */
} /* namespace SEMBA */

#include "Group.hpp"

namespace SEMBA {

typedef Source::Group<> SourceGroup;

} /* namespace SEMBA */

