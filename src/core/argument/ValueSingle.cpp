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

#include "ValueSingle.h"

#include <stdexcept>

namespace SEMBA {
namespace Argument {

void ValueSingle::parse(Object& out,
                        std::vector<std::list<std::string>>& output,
                        std::vector<std::list<std::string>>& input) const {
    if (input.size() != 1) {
        throw Error::Multiple(*this);
    }
    output.resize(1);
    parseSingle(out, output[0], input[0]);
}

} /* namespace Argument */
} /* namespace SEMBA */
