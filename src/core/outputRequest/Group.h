

#pragma once

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

    void printInfo() const;
};

} /* namespace OutputRequest */
} /* namespace SEMBA */

#include "Group.hpp"

