#pragma once

#include "Source.h"
#include "group/GroupIdentifiableUnique.h"

namespace SEMBA {
namespace Source {

template<typename S = Source>
class Group : public GroupIdentifiableUnique<S> {
};

} /* namespace Source */

using SourceGroup = Source::Group<>;

} /* namespace SEMBA */
