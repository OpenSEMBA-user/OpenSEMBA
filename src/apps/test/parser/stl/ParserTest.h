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
#ifndef PARSERSTLTEST_H_
#define PARSERGIDTEST_H_

#include "gtest/gtest.h"
#include "geometry/mesh/Geometric.h"
#include "geometry/element/Triangle3.h"
#include "parser/stl/Parser.h"
#include "exporter/vtk/Exporter.h"

using namespace std;
using namespace SEMBA;

class ParserSTLParserTest : public ::testing::Test {

protected:

    ParserSTLParserTest();

    virtual ~ParserSTLParserTest();

    string stlFolder_;
    Data* parseFromSTL(const string project) const;

};

#endif
