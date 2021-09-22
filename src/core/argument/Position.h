

#pragma once

#include "PositionBase.h"
#include "Value.h"
#include "ValueSingle.h"

namespace SEMBA {
namespace Argument {

template <typename... T>
class Position : public virtual PositionBase,
                 public virtual Value<ValueSingle, T...> {
public:
    Position(const std::string& name) : Argument(name) {}
    Position(const Position&    rhs)  : Argument(rhs ) {}
    virtual ~Position() {}
};

} /* namespace Argument */
} /* namespace SEMBA */

