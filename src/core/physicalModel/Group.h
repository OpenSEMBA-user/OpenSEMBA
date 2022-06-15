#pragma once

#include "PhysicalModel.h"
#include "class/GroupIdentifiableUnique.h"

namespace SEMBA {
namespace PhysicalModel {

template<typename P = PhysicalModel>
class Group : public Class::Group::GroupIdentifiableUnique<P> {
};

} /* namespace PhysicalModel */

typedef PhysicalModel::Group<> PMGroup;

} /* namespace SEMBA */

