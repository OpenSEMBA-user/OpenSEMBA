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

#ifndef SEMBA_PARSER_GID_PROBLEMSIZE_H_
#define SEMBA_PARSER_GID_PROBLEMSIZE_H_

#include <cstddef>

namespace SEMBA {
namespace Parser {
namespace GiD {

class ProblemSize {
public:
    std::size_t v;	    // Number of coordinate points.
    std::size_t lin2;  // # linear segment lines.
    std::size_t lin3;  // # quadratic segment lines.
    std::size_t tri3;  // # triangular elements.
    std::size_t tri6;  // #
    std::size_t qua4;
    std::size_t tet4;  // # tetrahedron elements.
    std::size_t tet10; // #
    std::size_t hex8;
    std::size_t mat;   // # materials.

    ProblemSize();
    ProblemSize operator=(const ProblemSize& rhs);
    void printInfo() const;
};

} /* namespace GiD */
} /* namespace Parser */
} /* namespace SEMBA */

#endif /* SEMBA_PARSER_GID_PROBLEMSIZE_H_ */
