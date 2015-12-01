
#ifndef SEMBA_ARGUMENT_MULTIOPTION_H_
#define SEMBA_ARGUMENT_MULTIOPTION_H_

#include "OptionValue.h"
#include "Value.h"
#include "ValueMulti.h"

namespace SEMBA {
namespace Argument {

template <typename... T>
class MultiOption : public OptionValue,
                    public Value<ValueMulti, T...> {
public:
    MultiOption(const std::string& name)
    :   Argument(name), OptionValue(name) {}
    MultiOption(const char& shortId, const std::string& longId)
    :   Argument(longId), OptionValue(shortId, longId) {}
    MultiOption(const std::string& name, const char& shortId)
    :   Argument(name), OptionValue(shortId) {}
    MultiOption(const std::string& name, const std::string& longId)
    :   Argument(name), OptionValue(longId) {}
    MultiOption(const std::string& name,
                const char& shortId, const std::string& longId)
    :   Argument(name), OptionValue(shortId, longId) {}
    MultiOption(const OptionValue& rhs)
    :   Argument(rhs), OptionValue(rhs) {}
    virtual ~MultiOption() {}

    MultiOption* clone() const { return new MultiOption(*this); }
};

} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_MULTIOPTION_H_ */
