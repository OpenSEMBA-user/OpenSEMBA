

#include "MultiSwitch.h"

namespace SEMBA {
namespace Argument {

MultiSwitch::MultiSwitch(const std::string& name)
:   Argument(name),
    OptionBase(name) {
    initDefault_();
}

MultiSwitch::MultiSwitch(const char& shortId, const std::string& longId)
:   Argument(longId),
    OptionBase(shortId, longId) {
    initDefault_();
}

MultiSwitch::MultiSwitch(const std::string& name, const char& shortId)
:   Argument(name),
    OptionBase(shortId) {
    initDefault_();
}

MultiSwitch::MultiSwitch(const std::string& name, const std::string& longId)
:   Argument(name),
    OptionBase(longId) {
    initDefault_();
}

MultiSwitch::MultiSwitch(const std::string& name,
                         const char& shortId,
                         const std::string& longId)
:   Argument(name),
    OptionBase(shortId, longId) {
    initDefault_();
}

MultiSwitch::MultiSwitch(const MultiSwitch& rhs)
:   Argument(rhs),
    SwitchBase(rhs) {
    hasDefault_ = rhs.hasDefault_;
    default_    = rhs.default_;
}

MultiSwitch::~MultiSwitch() {

}

MultiSwitch* MultiSwitch::required(const bool& required) {
    Argument::required(required);
    return this;
}

MultiSwitch* MultiSwitch::help(const std::string& help) {
    Argument::help(help);
    return this;
}

MultiSwitch* MultiSwitch::defaultVal(const std::size_t& defaultVal) {
    hasDefault_ = true;
    default_ = defaultVal;
    return this;
}

void MultiSwitch::parse(Object& out,
                        std::vector<std::list<std::string>>& output,
                        std::vector<std::list<std::string>>& input) const {
    out.set(default_ + input.size());
    output = input;
    input.clear();
}

void MultiSwitch::notParsed(Object& out) const {
    if (hasDefault_) {
        out.set(default_);
    } else {
        Argument::notParsed(out);
    }
}

void MultiSwitch::initDefault_() {
    hasDefault_ = false;
    default_    = 0;
}

} /* namespace Argument */
} /* namespace SEMBA */
