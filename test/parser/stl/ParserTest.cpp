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
#include "gtest/gtest.h"
#include "geometry/mesh/Geometric.h"
#include "geometry/element/Triangle3.h"
#include "parser/stl/Parser.h"
#include "exporter/vtk/Exporter.h"

using namespace std;
using namespace SEMBA;

class ParserSTLParserTest : public ::testing::Test {
protected:
    ParserSTLParserTest() {
        stlFolder_ = "./testData/";
    }

    string stlFolder_;
	string getCaseName(const string project) const {
		return stlFolder_ + project + ".stl";
	}
};

TEST_F(ParserSTLParserTest, case_nofile) {
	string project = "nofile";
	Parser::STL::Parser parser;
	ifstream input(getCaseName(project));
	ASSERT_NO_THROW(parser.read(input));
	Data smb = parser.read(input);
	EXPECT_TRUE(smb.mesh != nullptr);
}

TEST_F(ParserSTLParserTest, case_single) {
    string project = "single";
	Parser::STL::Parser parser;
	ifstream input(getCaseName(project));
	ASSERT_TRUE(input.is_open());
    Data smb = parser.read(input);
    ASSERT_TRUE(smb.mesh != nullptr);
    Geometry::Mesh::Geometric* mesh =
            smb.mesh->castTo<Geometry::Mesh::Geometric>();
    if (smb.mesh != nullptr) {
        EXPECT_EQ(3, mesh->coords().size());
        EXPECT_EQ(1, mesh->elems().getOf<Geometry::Tri3>().size());
    }
}

TEST_F(ParserSTLParserTest, case_B2) {
    string project = "B2";
	Parser::STL::Parser parser;
	ifstream input(getCaseName(project));
	Data smb = parser.read(input);
    ASSERT_TRUE(smb.mesh != nullptr);
    Geometry::Mesh::Geometric* mesh =
            smb.mesh->castTo<Geometry::Mesh::Geometric>();
    if (smb.mesh != nullptr) {
        EXPECT_EQ(1956, mesh->coords().size());
        EXPECT_EQ(652, mesh->elems().getOf<Geometry::Tri3>().size());
    }
}
