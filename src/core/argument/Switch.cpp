

#include "Switch.h"

namespace SEMBA {
namespace Argument {

Switch::Switch(const std::string& name)
:   Argument(name),
    OptionBase(name) {
    initDefault_();
}

Switch::Switch(const std::string& name, const char& shortId)
:   Argument(name),
    OptionBase(shortId) {
    initDefault_();
}

Switch::Switch(const char& shortId, const std::string& longId)
:   Argument(longId),
    OptionBase(shortId, longId) {
    initDefault_();
}

Switch::Switch(const std::string& name, const std::string& longId)
:   Argument(name),
    OptionBase(longId) {
    initDefault_();
}

Switch::Switch(const std::string& name,
               const char& shortId,
               const std::string& longId)
:   Argument(name),
    OptionBase(shortId, longId) {
    initDefault_();
}

Switch::Switch(const Switch& rhs)
:   Argument(rhs),
    OptionBase(rhs) {
    hasDefault_ = rhs.hasDefault_;
    default_    = rhs.default_;
}

Switch::~Switch() {

}

Switch* Switch::required(const bool& required) {
    Argument::required(required);
    return this;
}

Switch* Switch::help(const std::string& help) {
    Argument::help(help);
    return this;
}

Switch* Switch::defaultVal(const bool& defaultVal) {
    hasDefault_ = true;
    default_ = defaultVal;
    return this;
}

void Switch::parse(Object& out,
                   std::vector<std::list<std::string>>& output,
                   std::vector<std::list<std::string>>& input) const {
    if (input.size() != 1) {
        throw Error::Multiple(*this);
    }
    if (hasDefault_) {
        if (default_) {
            out.setFalse();
        } else {
            out.setTrue();
        }
    } else {
        out.setTrue();
    }
    output = input;
    input[0].clear();
}

void Switch::notParsed(Object& out) const {
    if (hasDefault_) {
        if (default_) {
            out.setTrue();
        } else {
            out.setFalse();
        }
    } else {
        Argument::notParsed(out);
    }
}

void Switch::initDefault_() {
    hasDefault_ = false;
    default_ = false;
}

} /* namespace Argument */
} /* namespace SEMBA */
