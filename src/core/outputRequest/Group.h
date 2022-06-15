#pragma once

#include "OutputRequest.h"
#include "class/GroupIdentifiableUnique.h"

namespace SEMBA {
namespace OutputRequest {

template<typename O = OutputRequest>
class Group : public Class::Group::GroupIdentifiableUnique<O> {
};

} /* namespace OutputRequest */

typedef OutputRequest::Group<> OutputRequestGroup;

} /* namespace SEMBA */

