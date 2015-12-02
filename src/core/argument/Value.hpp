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

#include "Value.h"

namespace SEMBA {
namespace Argument {

template<typename V, typename T1, typename... T>
void Value<V, T1, T...>::parseSingle(Object& out,
                                     std::list<std::string>& output,
                                     std::list<std::string>& input) const {
    StringParser::parse<T1, T...>(input, output);
    const std::vector<std::list<std::string>>& choices = this->getChoices();
    if (!choices.empty()) {
        bool found = true;
        for (std::vector<std::list<std::string>>::const_iterator
             it = choices.begin(); it != choices.end(); ++it) {
            found = true;
            if (it->size() != input.size()) {
                found = false;
                continue;
            }
            std::list<std::string>::const_iterator it1 = it->begin();
            std::list<std::string>::const_iterator it2 = input.begin();
            while ((it1 != it->end()) && (it2 != input.end())) {
                if ((*it1 != *it2) && (*it1 != "*")) {
                    found = false;
                    break;
                }
                ++it1;
                ++it2;
            }
            if (found) {
                break;
            }
        }
        if (!found) {
            throw Error::NotChoices(*this, choices);
        }
    }
    out.setGenericString(input);
}

template<typename V, typename T1, typename... T>
void Value<V, T1, T...>::noParsed(Object& out) const {
    const std::list<std::string>& defaultVar = this->getDefault();
    if (!defaultVar.empty()) {
        out.setGenericString(defaultVar);
    } else {
        Argument::noParsed(out);
    }
}

} /* namespace Argument */
} /* namespace SEMBA */
