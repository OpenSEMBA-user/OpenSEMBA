
#include "MultiSwitch.h"

namespace SEMBA {
namespace Argument {

MultiSwitch::MultiSwitch(const std::string& name)
:   Argument(name),
    SwitchBase(name) {
    initDefault_();
}

MultiSwitch::MultiSwitch(const char& shortId, const std::string& longId)
:   Argument(longId),
    SwitchBase(shortId, longId) {
    initDefault_();
}

MultiSwitch::MultiSwitch(const std::string& name, const char& shortId)
:   Argument(name),
    SwitchBase(shortId) {
    initDefault_();
}

MultiSwitch::MultiSwitch(const std::string& name, const std::string& longId)
:   Argument(name),
    SwitchBase(longId) {
    initDefault_();
}

MultiSwitch::MultiSwitch(const std::string& name,
                         const char& shortId,
                         const std::string& longId)
:   Argument(name),
    SwitchBase(shortId, longId) {
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

MultiSwitch& MultiSwitch::required(const bool& required) {
    Argument::required(required);
    return *this;
}

MultiSwitch& MultiSwitch::help(const std::string& help) {
    Argument::help(help);
    return *this;
}

MultiSwitch& MultiSwitch::defaultVal(const std::size_t& defaultVal) {
    hasDefault_ = true;
    default_ = defaultVal;
    return *this;
}

void MultiSwitch::parse(Object& out,
                        std::vector<std::list<std::string>>& output,
                        std::vector<std::list<std::string>>& input) const {
    out.setInt(default_ + input.size());
    output = input;
    input.clear();
}

void MultiSwitch::noParsed(Object& out) const {
    if (hasDefault_) {
        out.setInt(default_);
    } else {
        Argument::noParsed(out);
    }
}

void MultiSwitch::initDefault_() {
    hasDefault_ = false;
    default_    = 0;
}

} /* namespace Argument */
} /* namespace SEMBA */
