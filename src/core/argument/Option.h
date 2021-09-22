

#pragma once

#include "OptionValue.h"
#include "Value.h"
#include "ValueSingle.h"

namespace SEMBA {
namespace Argument {

template <typename... T>
class Option : public virtual OptionValue,
               public virtual Value<ValueSingle, T...> {
public:
    Option(const std::string& name)
    :   Argument(name), OptionBase(name) {}
    Option(const char& shortId, const std::string& longId)
    :   Argument(longId), OptionBase(shortId, longId) {}
    Option(const std::string& name, const char& shortId)
    :   Argument(name), OptionBase(shortId) {}
    Option(const std::string& name, const std::string& longId)
    :   Argument(name), OptionBase(longId) {}
    Option(const std::string& name,
           const char& shortId, const std::string& longId)
    :   Argument(name), OptionBase(shortId, longId) {}
    Option(const Option& rhs)
    :   Argument(rhs), OptionBase(rhs) {}
    virtual ~Option() {}
};

} /* namespace Argument */
} /* namespace SEMBA */

