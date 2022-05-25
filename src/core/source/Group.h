#pragma once

#include "Source.h"
#include "group/IdentifiableUnique.h"

namespace SEMBA {
namespace Source {

template<typename S = Source>
class Group : public IdentifiableUnique<S> {
};

} /* namespace Source */

typedef Source::Group<> SourceGroup;

} /* namespace SEMBA */
