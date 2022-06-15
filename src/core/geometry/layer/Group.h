#pragma once

#include "Layer.h"
#include "class/GroupIdentifiableUnique.h"

namespace SEMBA {
namespace Geometry {
namespace Layer {

template<typename L = Layer>
class Group final : public Class::Group::GroupIdentifiableUnique<L>  {
public:
    const L* getName(const std::string name) const;

};

} /* namespace Layer */

typedef Layer::Group<> LayerGroup;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Group.hpp"

