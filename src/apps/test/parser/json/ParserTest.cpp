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

#include "parser/json/Parser.h"

#include "geometry/element/Line2.h"
#include "geometry/element/Triangle3.h"
#include "geometry/element/Tetrahedron4.h"

using namespace SEMBA;
using namespace Parser::JSON;

class ParserJSONParserTest : public ::testing::Test {
};

TEST_F(ParserJSONParserTest, Basic) {
    std::istringstream stream(
            "{\n"
            "   \"_version\": \"" OPENSEMBA_VERSION "\"\n"
            "}"
    );

    SEMBA::Parser::JSON::Parser jsonParser;
    EXPECT_NO_THROW(jsonParser.read(stream));
}

TEST_F(ParserJSONParserTest, Cartesian) {
    std::ifstream stream("testData/cartesian.gid/cartesian.dat");
    EXPECT_TRUE(stream.is_open());

    SEMBA::Parser::JSON::Parser jsonParser;
    EXPECT_NO_THROW(jsonParser.read(stream));
}

TEST_F(ParserJSONParserTest, DMCWF) {
    std::ifstream stream("testData/dmcwf.gid/dmcwf.dat");
    EXPECT_TRUE(stream.is_open());

    SEMBA::Parser::JSON::Parser jsonParser;
    EXPECT_NO_THROW(jsonParser.read(stream));
}

TEST_F(ParserJSONParserTest, Planewave) {
    std::ifstream stream("testData/planewave.gid/planewave.dat");
    EXPECT_TRUE(stream.is_open());

    SEMBA::Parser::JSON::Parser jsonParser;
    EXPECT_NO_THROW(jsonParser.read(stream));
}

TEST_F(ParserJSONParserTest, Sphere) {
    std::ifstream stream("testData/sphere.gid/sphere.dat");
    EXPECT_TRUE(stream.is_open());

    SEMBA::Parser::JSON::Parser jsonParser;
    EXPECT_NO_THROW(jsonParser.read(stream));
}

TEST_F(ParserJSONParserTest, Wires) {
    std::ifstream stream("testData/wires.gid/wires.dat");
    EXPECT_TRUE(stream.is_open());

    SEMBA::Parser::JSON::Parser jsonParser;
    EXPECT_NO_THROW(jsonParser.read(stream));
}
