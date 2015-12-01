
#ifndef SEMBA_ARGUMENT_POSITIONBASE_H_
#define SEMBA_ARGUMENT_POSITIONBASE_H_

#include "Value.h"

namespace SEMBA {
namespace Argument {

class PositionBase : public virtual ValueBase {
public:
    PositionBase() { required(true); }
    virtual ~PositionBase() {}

    virtual PositionBase* clone() const = 0;

protected:
    ValueBase& required(const bool&) { return ValueBase::required(true); }
    using Argument::getRequired;
};

} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_POSITIONBASE_H_ */
