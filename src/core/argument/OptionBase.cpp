// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

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
