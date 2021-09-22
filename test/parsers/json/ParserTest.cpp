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

#include "parsers/json/Parser.h"

#include "geometry/element/Line2.h"
#include "geometry/element/Triangle3.h"
#include "geometry/element/Tetrahedron4.h"

using namespace SEMBA;
using namespace SEMBA::Parsers::JSON;
using namespace Geometry::Mesh;

class ParserJSONParserTest : public ::testing::Test {
};

TEST_F(ParserJSONParserTest, Cartesian) {
    SEMBA::Parsers::JSON::Parser jsonParser("testData/cartesian.gid/cartesian.dat");
    EXPECT_NO_THROW(jsonParser.read());
}

TEST_F(ParserJSONParserTest, DMCWF) {
    SEMBA::Parsers::JSON::Parser jsonParser("testData/dmcwf.gid/dmcwf.dat");
    EXPECT_NO_THROW(jsonParser.read());
}

TEST_F(ParserJSONParserTest, Planewave) {
    SEMBA::Parsers::JSON::Parser jsonParser("testData/planewave.gid/planewave.dat");
    EXPECT_NO_THROW(jsonParser.read());
}

TEST_F(ParserJSONParserTest, Sphere) {
    SEMBA::Parsers::JSON::Parser jsonParser("testData/sphere.gid/sphere.dat");
    EXPECT_NO_THROW(jsonParser.read());
}

TEST_F(ParserJSONParserTest, Wires) {
    SEMBA::Parsers::JSON::Parser jsonParser("testData/wires.gid/wires.dat");
    EXPECT_NO_THROW(jsonParser.read());
}

TEST_F(ParserJSONParserTest, Bowtie) {
    SEMBA::Parsers::JSON::Parser jsonParser("testData/bowtie.gid/bowtie.dat");
    Data data;
    EXPECT_NO_THROW(data = jsonParser.read());

    ASSERT_TRUE(data.mesh != nullptr);
    Geometric* unstrMesh = dynamic_cast<Geometric*>(data.mesh);
    EXPECT_FALSE(unstrMesh->grid().hasZeroSize());
}