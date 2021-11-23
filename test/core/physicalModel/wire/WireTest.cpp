#include "gtest/gtest.h"
#include "physicalModel/wire/Wire.h"

using namespace SEMBA;
using namespace PhysicalModel;

class PhysicalModelWireTest : public ::testing::Test {
public:

protected:

};

TEST_F(PhysicalModelWireTest, copy_assignment) 
{
    Wire::Wire orig(Id(2), "Cable", 1.0, 1.0, 1.0);

    Wire::Wire copied = orig;

    EXPECT_EQ(Id(2), copied.getId());
}

TEST_F(PhysicalModelWireTest, copy_ctor)
{
    Wire::Wire orig(Id(2), "Cable", 1.0, 1.0, 1.0);
    Wire::Wire copied(orig);

    EXPECT_EQ(Id(2), copied.getId());

    auto ptr = std::make_unique<SEMBA::PhysicalModel::PhysicalModel>(orig);
    EXPECT_EQ(Id(2), ptr->getId());

}