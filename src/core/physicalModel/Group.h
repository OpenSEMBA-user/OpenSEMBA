#pragma once

#include "PhysicalModel.h"
#include "group/IdentifiableUnique.h"

namespace SEMBA {
namespace PhysicalModel {

template<typename P = PhysicalModel>
class Group : public IdentifiableUnique<P> {
};

} /* namespace PhysicalModel */

typedef PhysicalModel::Group<> PMGroup;

} /* namespace SEMBA */

