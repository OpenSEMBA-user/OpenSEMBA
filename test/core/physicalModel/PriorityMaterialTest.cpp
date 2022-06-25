#include "gtest/gtest.h"
#include "physicalModel/PriorityMaterial.h"

using namespace SEMBA;
using namespace PhysicalModel;

class PriorityMaterialTest : public ::testing::Test {
};

TEST_F(PriorityMaterialTest, assignation)
{
    PriorityMaterial a{ Id{ 3 }, "Test", 3000, true };
    
    PriorityMaterial c{ a };
    EXPECT_EQ(a, c);

    PriorityMaterial b{ Id{ 3 }, "Test", 5000, false };
    c = b;
    EXPECT_EQ(b, c);

    EXPECT_FALSE(a == b);
}

