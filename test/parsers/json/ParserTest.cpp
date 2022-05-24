
#include "gtest/gtest.h"

#include "parsers/json/Parser.h"

#include "geometry/element/Line2.h"
#include "geometry/element/Triangle3.h"
#include "geometry/element/Tetrahedron4.h"

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
