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

#ifndef SEMBA_ARGUMENT_VALUEBASE_H_
#define SEMBA_ARGUMENT_VALUEBASE_H_

#include "Argument.h"

namespace SEMBA {
namespace Argument {

class ValueBase : public virtual Argument {
public:
    ValueBase() {}
    virtual ~ValueBase() {}

    using Argument::getMetavar;
    using Argument::getDefault;
    using Argument::getChoices;

    ValueBase& required  (const bool& = true);
    ValueBase& help      (const std::string&);
    ValueBase& metavar   (const std::list<std::string>&);
    ValueBase& defaultVal(const std::list<std::string>&);
    ValueBase& choices   (const std::vector<std::list<std::string>>&);
};

} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_VALUEBASE_H_ */
