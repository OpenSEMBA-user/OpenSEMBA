

#pragma once

#include "Value.h"

namespace SEMBA {
namespace Argument {

class PositionBase : public virtual ValueBase {
public:
    PositionBase() { required(true); }
    virtual ~PositionBase() {}

protected:
    Argument* required(const bool&) { return Argument::required(true); }
    using Argument::getRequired;
};

} /* namespace Argument */
} /* namespace SEMBA */

