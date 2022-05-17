
#include "gtest/gtest.h"

#include "parsers/json/Parser.h"

#include "geometry/element/Line2.h"
#include "geometry/element/Triangle3.h"
#include "geometry/element/Tetrahedron4.h"
#include "DataExtended.h"

using namespace SEMBA;
using namespace SEMBA::Parsers::JSON;
using namespace Geometry::Mesh;

class ParserJSONParserTest : public ::testing::Test {
};

TEST_F(ParserJSONParserTest, DMCWF) 
{
    SEMBA::Parsers::JSON::Parser jsonParser("testData/dmcwf.gid/dmcwf.dat");
    EXPECT_NO_THROW(jsonParser.read());
}

TEST_F(ParserJSONParserTest, Sphere) 
{
    SEMBA::Parsers::JSON::Parser jsonParser("testData/sphere.gid/sphere.dat");
    EXPECT_NO_THROW(jsonParser.read());
}

TEST_F(ParserJSONParserTest, SphereExtended)
{
    SEMBA::Parsers::JSON::Parser jsonParser("testData/sphere.gid/sphere-extended.dat");

    DataExtended data = jsonParser.readExtended();

    const std::string boundaryLowerMaterials[3] = {"PML", "PEC", "PMC"};
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(data.boundary->lower[i], boundaryLowerMaterials[i]);
    }

    const std::string boundaryUpperMaterials[3] = {"MUR1", "MUR2", "Periodic"};
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(data.boundary->upper[i], boundaryUpperMaterials[i]);
    }

    EXPECT_EQ(data.grid3->getNumCells(), Math::CVecR3(51, 23, 15));
}

TEST_F(ParserJSONParserTest, SphereExtendedWithWrongSubversion)
{
    SEMBA::Parsers::JSON::Parser jsonParser("testData/sphere.gid/sphere-extended-wrong-subversion.dat");
    try {
        jsonParser.read();
        FAIL() << "No exception was thrown";
    }
    catch (const std::logic_error& exception) {
        EXPECT_STREQ(exception.what(), "File version 0.15a is not supported.");
    }
}

TEST_F(ParserJSONParserTest, Wires) 
{
    SEMBA::Parsers::JSON::Parser jsonParser("testData/wires.gid/wires.dat");
    SEMBA::Data data = jsonParser.read();
}

TEST_F(ParserJSONParserTest, Bowtie) 
{
    SEMBA::Parsers::JSON::Parser jsonParser("testData/bowtie.gid/bowtie.dat");
    Data data;
    EXPECT_NO_THROW(data = jsonParser.read());

    ASSERT_TRUE(data.mesh != nullptr);
    Geometric unstrMesh = dynamic_cast<Geometric&>(*data.mesh);
    EXPECT_FALSE(unstrMesh.grid().hasZeroSize());
}
