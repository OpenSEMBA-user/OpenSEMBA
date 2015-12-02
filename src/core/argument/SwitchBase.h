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

#ifndef SEMBA_ARGUMENT_SWITCHBASE_H_
#define SEMBA_ARGUMENT_SWITCHBASE_H_

#include "OptionBase.h"

namespace SEMBA {
namespace Argument {

class SwitchBase : public OptionBase {
public:
    SwitchBase(const char& shortId)
    :   OptionBase(shortId) {}
    SwitchBase(const std::string& longId)
    :   OptionBase(longId) {}
    SwitchBase(const char& shortId, const std::string& longId)
    :   OptionBase(shortId, longId) {}
    SwitchBase(const SwitchBase& rhs)
    :   OptionBase(rhs) {}
    virtual ~SwitchBase() {}

    virtual SwitchBase* clone() const = 0;
};

} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_SWITCHBASE_H_ */
