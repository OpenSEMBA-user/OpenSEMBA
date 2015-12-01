
#ifndef SEMBA_ARGUMENT_OPTION_H_
#define SEMBA_ARGUMENT_OPTION_H_

#include "OptionValue.h"
#include "Value.h"
#include "ValueSingle.h"

namespace SEMBA {
namespace Argument {

template <typename... T>
class Option : public OptionValue,
               public Value<ValueSingle, T...> {
public:
    Option(const std::string& name)
    :   Argument(name), OptionValue(name) {}
    Option(const char& shortId, const std::string& longId)
    :   Argument(longId), OptionValue(shortId, longId) {}
    Option(const std::string& name, const char& shortId)
    :   Argument(name), OptionValue(shortId) {}
    Option(const std::string& name, const std::string& longId)
    :   Argument(name), OptionValue(longId) {}
    Option(const std::string& name,
           const char& shortId, const std::string& longId)
    :   Argument(name), OptionValue(shortId, longId) {}
    Option(const OptionValue& rhs)
    :   Argument(rhs), OptionValue(rhs) {}
    virtual ~Option() {}

    Option* clone() const { return new Option(*this); }
};

} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_OPTION_H_ */
