
#ifndef SEMBA_ARGUMENT_MULTIPOSITION_H_
#define SEMBA_ARGUMENT_MULTIPOSITION_H_

#include "PositionBase.h"
#include "Value.h"
#include "ValueMulti.h"

namespace SEMBA {
namespace Argument {

template <typename... T>
class MultiPosition : public PositionBase,
                      public Value<ValueMulti, T...> {
public:
    MultiPosition(const std::string&   name) : Argument(name) {}
    MultiPosition(const MultiPosition& rhs)  : Argument(rhs ) {}
    virtual ~MultiPosition() {}

    MultiPosition* clone() const { return new MultiPosition(*this); }
};

} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_MULTIPOSITION_H_ */
