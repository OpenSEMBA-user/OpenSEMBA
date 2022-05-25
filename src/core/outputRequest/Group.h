#pragma once

#include "OutputRequest.h"
#include "group/IdentifiableUnique.h"

namespace SEMBA {
namespace OutputRequest {

template<typename O = OutputRequest>
class Group : public IdentifiableUnique<O> {
};

} /* namespace OutputRequest */

typedef OutputRequest::Group<> OutputRequestGroup;

} /* namespace SEMBA */

