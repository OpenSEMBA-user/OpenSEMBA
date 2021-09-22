

#pragma once

#ifdef _MSC_VER
#pragma warning(disable:4250)
#endif

#include "OptionBase.h"
#include "ValueBase.h"

namespace SEMBA {
namespace Argument {

class OptionValue : public virtual OptionBase,
                    public virtual ValueBase {
public:
    OptionValue() {}
    virtual ~OptionValue() {}
};

} /* namespace Argument */
} /* namespace SEMBA */

