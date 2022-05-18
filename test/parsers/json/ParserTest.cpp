
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
protected:
    static std::string getFilename(const std::string& name)
    {
        return "testData/" + name + ".gid/" + name + ".dat";
    }
};

TEST_F(ParserJSONParserTest, DMCWF) 
{
    ASSERT_NO_THROW(Parser(getFilename("dmcwf")).read());
}

TEST_F(ParserJSONParserTest, Sphere) 
{
    ASSERT_NO_THROW(Parser(getFilename("sphere")).read());
}

TEST_F(ParserJSONParserTest, SphereExtended)
{
    SEMBA::Parsers::JSON::Parser jsonParser("testData/sphere.gid/sphere-extended.dat");

    DataExtended data = jsonParser.readExtended();

    const PhysicalModel::Bound::Type boundaryLowerMaterials[3] = {
        PhysicalModel::Bound::Type::pml,
        PhysicalModel::Bound::Type::pec,
        PhysicalModel::Bound::Type::pmc
    };
    const PhysicalModel::Bound::Type boundaryUpperMaterials[3] = {
        PhysicalModel::Bound::Type::mur1,
        PhysicalModel::Bound::Type::mur2,
        PhysicalModel::Bound::Type::periodic
    };

    int i = 0;
    for (auto& pairs : data.boundary) {
        EXPECT_EQ(pairs.first.getType(), boundaryLowerMaterials[i]);
        EXPECT_EQ(pairs.second.getType(), boundaryUpperMaterials[i]);
        i++;
    }

    EXPECT_EQ(data.grid3.getNumCells(), Math::CVecR3(51, 23, 15));
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
    ASSERT_NO_THROW(Parser(getFilename("wires")).read());
}

TEST_F(ParserJSONParserTest, Bowtie) 
{
    Data data;
    ASSERT_NO_THROW(data = Parser(getFilename("bowtie")).read());

    ASSERT_TRUE(data.mesh != nullptr);
    Geometric unstrMesh = dynamic_cast<Geometric&>(*data.mesh);
    EXPECT_FALSE(unstrMesh.grid().hasZeroSize());
}

TEST_F(ParserJSONParserTest, B2)
{
    Data data;
    ASSERT_NO_THROW(data = Parser(getFilename("b2")).read());

    ASSERT_TRUE(data.mesh != nullptr);
    Geometric unstrMesh = dynamic_cast<Geometric&>(*data.mesh);
    EXPECT_FALSE(unstrMesh.grid().hasZeroSize());
    EXPECT_EQ(652, unstrMesh.elems().sizeOf<Geometry::Tri3>());
}
