#pragma once

#include "PhysicalModel.h"
#include "group/GroupIdentifiableUnique.h"

namespace SEMBA {
namespace PhysicalModel {

template<typename P = PhysicalModel>
class Group : public GroupIdentifiableUnique<P> {
};

} /* namespace PhysicalModel */

typedef PhysicalModel::Group<> PMGroup;

} /* namespace SEMBA */

