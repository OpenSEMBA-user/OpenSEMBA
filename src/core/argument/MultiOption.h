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

#include "OptionValue.h"
#include "Value.h"
#include "ValueMulti.h"

namespace SEMBA {
namespace Argument {

template <typename... T>
class MultiOption : public virtual OptionValue,
                    public virtual Value<ValueMulti, T...> {
public:
    MultiOption(const std::string& name)
    :   Argument(name), OptionBase(name) {}
    MultiOption(const char& shortId, const std::string& longId)
    :   Argument(longId), OptionBase(shortId, longId) {}
    MultiOption(const std::string& name, const char& shortId)
    :   Argument(name), OptionBase(shortId) {}
    MultiOption(const std::string& name, const std::string& longId)
    :   Argument(name), OptionBase(longId) {}
    MultiOption(const std::string& name,
                const char& shortId, const std::string& longId)
    :   Argument(name), OptionBase(shortId, longId) {}
    MultiOption(const MultiOption& rhs)
    :   Argument(rhs), OptionBase(rhs) {}
    virtual ~MultiOption() {}
};

} /* namespace Argument */
} /* namespace SEMBA */

