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

#pragma once

#include <utility>
#include <tuple>

#include "Argument.h"
#include "Object.h"
#include "StringParser.h"

namespace SEMBA {
namespace Argument {

template <typename V, typename T1, typename... T>
class Value : public virtual V {
public:
    Value() {}
    virtual ~Value() {}

    void parseSingle(Object&,
                     std::list<std::string>&,
                     std::list<std::string>&) const;

    virtual void notParsed(Object&) const;
};

namespace Error {

class NotChoices : public Error {
public:
    NotChoices(const Argument& arg,
               const std::vector<std::list<std::string>>& c)
    :   arg_(arg),
        choices_(c) {
        std::stringstream aux;
        aux << "Argument " << arg.getArgName()
            << " not matching possible choices"
            << " (choose from";
        for (std::size_t i = 0; i < c.size(); i++) {
            if (i != 0) {
                aux << " ,";
            }
            for (std::list<std::string>::const_iterator
                 it = c[i].begin(); it != c[i].end(); ++it) {
                aux << " " << *it;
            }
        }
        aux << ")";
    }
    virtual ~NotChoices() throw() {}

    const char* what() const throw() { return str_.c_str(); }

    const Argument& getArg() const { return arg_; }
    const std::vector<std::list<std::string>>& getChoices() const {
        return choices_;
    }
private:
    const Argument& arg_;
    std::vector<std::list<std::string>> choices_;
    std::string str_;
};

} /* namespace Error */
} /* namespace Argument */
} /* namespace SEMBA */

#include "Value.hpp"

