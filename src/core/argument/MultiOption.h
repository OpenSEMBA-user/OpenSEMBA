

#pragma once

#include "OptionValue.h"
#include "Value.h"
#include "ValueMulti.h"

namespace SEMBA {
namespace Argument {

template <typename... T>
class MultiOption : public virtual OptionValue,
                    public virtual Value<ValueMulti, T...> {
public:
    MultiOption(const std::string& name)
    :   Argument(name), OptionBase(name) {}
    MultiOption(const char& shortId, const std::string& longId)
    :   Argument(longId), OptionBase(shortId, longId) {}
    MultiOption(const std::string& name, const char& shortId)
    :   Argument(name), OptionBase(shortId) {}
    MultiOption(const std::string& name, const std::string& longId)
    :   Argument(name), OptionBase(longId) {}
    MultiOption(const std::string& name,
                const char& shortId, const std::string& longId)
    :   Argument(name), OptionBase(shortId, longId) {}
    MultiOption(const MultiOption& rhs)
    :   Argument(rhs), OptionBase(rhs) {}
    virtual ~MultiOption() {}
};

} /* namespace Argument */
} /* namespace SEMBA */

