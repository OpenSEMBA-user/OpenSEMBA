#include "gtest/gtest.h"
#include "physicalModel/wire/Wire.h"

using namespace SEMBA;
using namespace PhysicalModel;

class PhysicalModelWireTest : public ::testing::Test {};

TEST_F(PhysicalModelWireTest, copy_assignment) 
{
    Wire::Wire orig(Id(2), "Cable", 1.0, 1.0, 1.0);

    Wire::Wire copied = orig;

    EXPECT_EQ(Id(2), copied.getId());
    EXPECT_EQ("Cable", copied.getName());
}

TEST_F(PhysicalModelWireTest, copy_ctor)
{
    Wire::Wire orig(Id(2), "Cable", 1.0, 1.0, 1.0);
    Wire::Wire copied(orig);
    
    EXPECT_EQ(Id(2), orig.getId());


    EXPECT_EQ(Id(2), copied.getId());
    EXPECT_EQ("Cable", copied.getName());
}

TEST_F(PhysicalModelWireTest, build_with_base_make_unique_ptr)
{
    Wire::Wire orig(Id(2), "Cable", 1.0, 1.0, 1.0);

    auto ptr = std::make_unique<Wire::Wire>(orig);

    EXPECT_EQ(Id(2), ptr->getId());
    EXPECT_EQ(std::string{ "Cable" }, ptr->getName());

    auto ptrCasted = ptr->castTo<Wire::Wire>();
    EXPECT_EQ(1.0, ptrCasted->getRadius());
    EXPECT_EQ(1.0, ptrCasted->getSeriesResistance());
    EXPECT_EQ(1.0, ptrCasted->getSeriesInductance());
}