
#include "Argument.h"

namespace SEMBA {
namespace Argument {

Argument::Argument() {
    initDefault_();
}

Argument::Argument(const std::string& name) {
    name_ = name;
    initDefault_();
}

Argument::Argument(const Argument& rhs) {
    name_     = rhs.name_;
    required_ = rhs.required_;
    help_     = rhs.help_;
    metavar_  = rhs.metavar_;
    default_  = rhs.default_;
    choices_  = rhs.choices_;
}

Argument::~Argument() {

}

Argument& Argument::required(const bool& required) {
    required_ = required;
    return *this;
}

Argument& Argument::help(const std::string& help) {
    help_ = help;
    return *this;
}

Argument& Argument::metavar(const std::list<std::string>& metavar) {
    metavar_ = metavar;
    return *this;
}

Argument& Argument::defaultVal(const std::list<std::string>& defaultVal) {
    default_ = defaultVal;
    return *this;
}

Argument& Argument::choices(const std::vector<std::list<std::string>>& choices) {
    choices_ = choices;
    return *this;
}

void Argument::noParsed(Object&) const {
    if (required_) {
        throw Error::Required(*this);
    }
}

void Argument::initDefault_() {
    required_ = false;
}

} /* namespace Argument */
} /* namespace SEMBA */
