#include "gtest/gtest.h"
#include "physicalModel/multiport/RLC.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Multiport {

class RLCTest : public ::testing::Test {
public:

protected:

};

TEST_F(RLCTest, ctor_and_copy)
{
	RLC orig(Id{ 1 }, "mp1", Multiport::Type::openCircuit, 0.0, 0.0, 0.0);

	EXPECT_EQ(Id{ 1 },                      orig.getId());
	EXPECT_EQ("mp1",                        orig.getName());
	EXPECT_EQ(Multiport::Type::openCircuit, orig.getType());

	RLC copied(orig);

	EXPECT_EQ(Id{ 1 },                      copied.getId());
	EXPECT_EQ("mp1",                        copied.getName());
	EXPECT_EQ(Multiport::Type::openCircuit, copied.getType());
}

}
}
}
