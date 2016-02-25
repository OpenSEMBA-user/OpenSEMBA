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
#include "ParserTest.h"

using namespace std;
using namespace SEMBA;

ParserSTLParserTest::ParserSTLParserTest() {
    stlFolder_ = "./testData/";
}

ParserSTLParserTest::~ParserSTLParserTest() {
}

SEMBA::Data* ParserSTLParserTest::parseFromSTL(const string project) const {
    Parser::STL::Parser parser(stlFolder_ + project + ".stl");
    EXPECT_TRUE(parser.canOpen());
    Data* res = parser.read();
    EXPECT_TRUE(res != NULL);
    if (res != NULL) {
        EXPECT_TRUE(res->check());
    }
    return res;
}


TEST_F(ParserSTLParserTest, case_single) {
    string project = "single";
    Data* smb = parseFromSTL(project);
    EXPECT_TRUE(smb->mesh != NULL);
    Geometry::Mesh::Geometric* mesh =
            smb->mesh->castTo<Geometry::Mesh::Geometric>();
    if (smb->mesh != NULL) {
        EXPECT_EQ(3, mesh->coords().size());
        EXPECT_EQ(1, mesh->elems().getOf<Geometry::Tri3>().size());
    }
}

TEST_F(ParserSTLParserTest, case_B2) {
    string project = "B2";
    Data* smb = parseFromSTL(project);
    EXPECT_TRUE(smb->mesh != NULL);
    Geometry::Mesh::Geometric* mesh =
            smb->mesh->castTo<Geometry::Mesh::Geometric>();
    if (smb->mesh != NULL) {
        EXPECT_EQ(1956, mesh->coords().size());
        EXPECT_EQ(652, mesh->elems().getOf<Geometry::Tri3>().size());
    }
}
