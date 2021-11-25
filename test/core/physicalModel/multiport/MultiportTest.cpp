#include "gtest/gtest.h"
#include "physicalModel/multiport/Multiport.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Multiport {

class MultiportTest : public ::testing::Test {
public:

protected:

};

TEST_F(MultiportTest, ctor_and_copy)
{
	Multiport orig(Id{ 1 }, "mp1", Multiport::Type::openCircuit);

	EXPECT_EQ(Id{ 1 },                      orig.getId());
	EXPECT_EQ("mp1",                        orig.getName());
	EXPECT_EQ(Multiport::Type::openCircuit, orig.getType());

	Multiport copied(orig);

	EXPECT_EQ(Id{ 1 },                      copied.getId());
	EXPECT_EQ("mp1",                        copied.getName());
	EXPECT_EQ(Multiport::Type::openCircuit, copied.getType());
}

}
}
}
