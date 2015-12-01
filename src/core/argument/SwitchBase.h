
#ifndef SEMBA_ARGUMENT_SWITCHBASE_H_
#define SEMBA_ARGUMENT_SWITCHBASE_H_

#include "OptionBase.h"

namespace SEMBA {
namespace Argument {

class SwitchBase : public OptionBase {
public:
    SwitchBase(const char& shortId)
    :   OptionBase(shortId) {}
    SwitchBase(const std::string& longId)
    :   OptionBase(longId) {}
    SwitchBase(const char& shortId, const std::string& longId)
    :   OptionBase(shortId, longId) {}
    SwitchBase(const SwitchBase& rhs)
    :   OptionBase(rhs) {}
    virtual ~SwitchBase() {}

    virtual SwitchBase* clone() const = 0;
};

} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_SWITCHBASE_H_ */
