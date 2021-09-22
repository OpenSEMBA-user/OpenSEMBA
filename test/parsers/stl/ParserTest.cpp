
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
