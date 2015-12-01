
#ifndef SEMBA_ARGUMENT_OPTIONVALUE_H_
#define SEMBA_ARGUMENT_OPTIONVALUE_H_

#include "OptionBase.h"
#include "ValueBase.h"

namespace SEMBA {
namespace Argument {

class OptionValue : public OptionBase,
                    public virtual ValueBase {
public:
    OptionValue(const char& shortId)
    :   OptionBase(shortId) {}
    OptionValue(const std::string& longId)
    :   OptionBase(longId) {}
    OptionValue(const char& shortId, const std::string& longId)
    :   OptionBase(shortId, longId) {}
    OptionValue(const OptionValue& rhs)
    :   OptionBase(rhs) {}
    virtual ~OptionValue() {}

    virtual OptionValue* clone() const = 0;
};

} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_OPTIONVALUE_H_ */
