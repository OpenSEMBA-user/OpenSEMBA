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

#include "ProblemSize.h"

#include <iostream>

namespace SEMBA {
namespace Parser {
namespace GiD {

ProblemSize::ProblemSize() {
    v = 0;
    lin2 = 0;
    lin3 = 0;
    tri3 = 0;
    tri6 = 0;
    qua4 = 0;
    tet4 = 0;
    tet10 = 0;
    hex8 = 0;
    mat = 0;
}

void ProblemSize::printInfo() const {
    std::cout << " --- Problem size --- " << std::endl;
    std::cout << "Number of coordinates:  " << v << std::endl;
    std::cout << "Number of materials:    " << mat << std::endl;
    std::cout << "Number of hex8:         " << hex8 << std::endl;
    std::cout << "Number of tet10:        " << tet10 << std::endl;
    std::cout << "Number of tet4:         " << tet4 << std::endl;
    std::cout << "Number of tri6:         " << tri6 << std::endl;
    std::cout << "Number of tri3:         " << tri3 << std::endl;
    std::cout << "Number of lin3:         " << lin3 << std::endl;
    std::cout << "Number of lin2:         " << lin2 << std::endl;
    std::cout << " --- End of Problem size --- " << std::endl;
}

} /* namespace GiD */
} /* namespace Parser */
} /* namespace SEMBA */
