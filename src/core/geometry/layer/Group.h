

#pragma once

#include "Layer.h"
#include "group/Cloneable.h"
#include "group/Identifiable.h"

namespace SEMBA {
namespace Geometry {
namespace Layer {

template<typename L = Layer>
class Group : public SEMBA::Group::Cloneable<L>,
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

};

} /* namespace Layer */

typedef Layer::Group<> LayerGroup;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Group.hpp"

