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
