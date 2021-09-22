

#include "OptionBase.h"

#include <cctype>

namespace SEMBA {
namespace Argument {

OptionBase::OptionBase()  {
    hasShortId_ = false;
    shortId_    = ' ';
    hasLongId_  = false;
    check_();
}

OptionBase::OptionBase(const char& shortId) {
    hasShortId_ = false;
    shortId_    = shortId;
    hasLongId_  = false;
    check_();
}

OptionBase::OptionBase(const std::string& longId) {
    hasShortId_ = false;
    shortId_    = ' ';
    hasLongId_  = false;
    longId_     = longId;
    check_();
}

OptionBase::OptionBase(const char& shortId,
                       const std::string& longId) {
    hasShortId_ = false;
    shortId_    = shortId;
    hasLongId_  = false;
    longId_     = longId;
    check_();
}

OptionBase::OptionBase(const OptionBase& rhs) {
    hasShortId_ = rhs.hasShortId_;
    shortId_    = rhs.shortId_;
    hasLongId_  = rhs.hasLongId_;
    longId_     = rhs.longId_;
}

OptionBase::~OptionBase() {

}

std::string OptionBase::getArgName() const {
    if (hasShortId_ && hasLongId_) {
        return std::string("-")  + std::string(1,shortId_) + std::string("/") +
               std::string("--") + longId_;
    }
    if (hasShortId_) {
        return std::string("-") + std::string(1,shortId_);
    }
    return std::string("--") + longId_;
}

void OptionBase::check_() {
    hasShortId_ = false;
    if (std::isalpha(shortId_)) {
        hasShortId_ = true;
    }
    hasLongId_ = false;
    if (longId_.size() > 1) {
        if (std::isalpha(longId_[0]) && std::isalpha(longId_[1])) {
            hasLongId_ = true;
        }
    }
    if (!hasShortId_ && !hasLongId_) {
        throw Error::NoIdentifier(*this);
    }
}

} /* namespace Argument */
} /* namespace SEMBA */
