#pragma once

#include "OutputRequest.h"
#include "group/GroupIdentifiableUnique.h"

namespace SEMBA {
namespace OutputRequest {

template<typename O = OutputRequest>
class Group : public GroupIdentifiableUnique<O> {
};

} /* namespace OutputRequest */

typedef OutputRequest::Group<> OutputRequestGroup;

} /* namespace SEMBA */

