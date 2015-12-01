
#include "ValueBase.h"

namespace SEMBA {
namespace Argument {

ValueBase& ValueBase::required(const bool& required) {
    Argument::required(required);
    return *this;
}

ValueBase& ValueBase::help(const std::string& help) {
    Argument::help(help);
    return *this;
}

ValueBase& ValueBase::metavar(
        const std::list<std::string>& metavar) {
    Argument::metavar(metavar);
    return *this;
}

ValueBase& ValueBase::defaultVal(
        const std::list<std::string>& defaultVal) {
    Argument::defaultVal(defaultVal);
    return *this;
}

ValueBase& ValueBase::choices(
        const std::vector<std::list<std::string>>& choices) {
    Argument::choices(choices);
    return *this;
}

} /* namespace Argument */
} /* namespace SEMBA */
