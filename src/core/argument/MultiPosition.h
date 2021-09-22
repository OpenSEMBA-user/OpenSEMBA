

#pragma once

#include "PositionBase.h"
#include "Value.h"
#include "ValueMulti.h"

namespace SEMBA {
namespace Argument {

template <typename... T>
class MultiPosition : public virtual PositionBase,
                      public virtual Value<ValueMulti, T...> {
public:
    MultiPosition(const std::string&   name) : Argument(name) {}
    MultiPosition(const MultiPosition& rhs)  : Argument(rhs ) {}
    virtual ~MultiPosition() {}
};

} /* namespace Argument */
} /* namespace SEMBA */

