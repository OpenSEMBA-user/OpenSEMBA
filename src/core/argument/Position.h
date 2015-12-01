
#ifndef SEMBA_ARGUMENT_POSITION_H_
#define SEMBA_ARGUMENT_POSITION_H_

#include "PositionBase.h"
#include "Value.h"
#include "ValueSingle.h"

namespace SEMBA {
namespace Argument {

template <typename... T>
class Position : public PositionBase,
                 public Value<ValueSingle, T...> {
public:
    Position(const std::string& name) : Argument(name) {}
    Position(const Position&    rhs)  : Argument(rhs ) {}
    virtual ~Position() {}

    Position* clone() const { return new Position(*this); }
};

} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_POSITION_H_ */
