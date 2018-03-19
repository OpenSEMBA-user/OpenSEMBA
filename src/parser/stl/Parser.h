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

#ifndef SEMBA_PARSER_STL_PARSER_H_
#define SEMBA_PARSER_STL_PARSER_H_

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "Data.h"
#include "geometry/element/Triangle3.h"
#include "geometry/mesh/Geometric.h"
#include "physicalModel/predefined/PEC.h"
#include "parser/Parser.h"


namespace SEMBA {
namespace Parser {
namespace STL {

class Parser : public SEMBA::Parser::Parser, public FileSystem::Project {
public:
    Parser();
    Parser(const std::string& fn);
    virtual ~Parser();

    Data read(std::istream& inputStream) const;

    void printInfo() const;
};

} /* namespace STL */
} /* namespace Parser */
} /* namespace SEMBA */

#endif /* SEMBA_PARSER_STL_PARSER_H_ */
