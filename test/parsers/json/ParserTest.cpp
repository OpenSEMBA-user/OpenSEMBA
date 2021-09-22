
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