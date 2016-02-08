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

Switch& Switch::required(const bool& required) {
    Argument::required(required);
    return *this;
}

Switch& Switch::help(const std::string& help) {
    Argument::help(help);
    return *this;
}

Switch& Switch::defaultVal(const bool& defaultVal) {
    hasDefault_ = true;
    default_ = defaultVal;
    return *this;
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

void Switch::noParsed(Object& out) const {
    if (hasDefault_) {
        if (default_) {
            out.setTrue();
        } else {
            out.setFalse();
        }
    } else {
        Argument::noParsed(out);
    }
}

void Switch::initDefault_() {
    hasDefault_ = false;
    default_ = false;
}

} /* namespace Argument */
} /* namespace SEMBA */
