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
#include "parsers/stl/Parser.h"
#include "exporters/vtk/Exporter.h"

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
	Parsers::STL::Parser parser("nofile");
	Data smb;
	ASSERT_NO_THROW(smb = parser.read());
	EXPECT_TRUE(smb.mesh != nullptr);
}

TEST_F(ParserSTLParserTest, case_single) {
    Parsers::STL::Parser parser(getCaseName("single"));
	Data smb = parser.read();
    ASSERT_TRUE(smb.mesh != nullptr);
    Geometry::Mesh::Geometric* mesh = smb.mesh->castTo<Geometry::Mesh::Geometric>();
    if (smb.mesh != nullptr) {
        EXPECT_EQ(3, mesh->coords().size());
        EXPECT_EQ(1, mesh->elems().getOf<Geometry::Tri3>().size());
    }
}

TEST_F(ParserSTLParserTest, case_B2) {
    Parsers::STL::Parser parser(getCaseName("B2"));
	Data smb = parser.read();
    ASSERT_TRUE(smb.mesh != nullptr);
    Geometry::Mesh::Geometric* mesh = smb.mesh->castTo<Geometry::Mesh::Geometric>();
    if (smb.mesh != nullptr) {
        EXPECT_EQ(1956, mesh->coords().size());
        EXPECT_EQ(652, mesh->elems().getOf<Geometry::Tri3>().size());
    }
}
