#include "gtest/gtest.h"

#include "group/IdentifiableUnique.h"
#include "geometry/layer/layer.h"

using namespace SEMBA;
using namespace Geometry;

class IdentifiableUniqueTest : public ::testing::Test {
public:
    
protected:
    auto newLayers() const 
    {
        IdentifiableUnique<Layer::Layer> res;
        res.add(std::make_unique<Layer::Layer>(LayerId(1), "Patata"));
        res.add(std::make_unique<Layer::Layer>(LayerId(2), "Cebolla"));
        res.add(std::make_unique<Layer::Layer>(LayerId(3), "Huevos"));
        return res;
    }
};

TEST_F(IdentifiableUniqueTest, copy_and_move_ctor) 
{
    auto orig = newLayers();
    std::size_t origSize = orig.size();

    auto copied( orig );
    EXPECT_EQ(origSize, copied.size());

    auto moved(std::move(orig));
    EXPECT_EQ(origSize, moved.size());
    EXPECT_EQ(origSize, copied.size());
    EXPECT_EQ(0, orig.size());
}
TEST_F(IdentifiableUniqueTest, copy_and_move_assignment) 
{
    auto orig = newLayers();
    std::size_t origSize = orig.size();
    
    auto copied = orig;
    EXPECT_EQ(origSize, copied.size());
    
    auto moved = std::move(orig);
    EXPECT_EQ(origSize, moved.size());
    EXPECT_EQ(origSize, copied.size());
    EXPECT_EQ(0, orig.size());
}

TEST_F(IdentifiableUniqueTest, deep_copy_and_move_add) 
{
    auto orig = newLayers();
    orig.add(std::make_unique<Layer::Layer>(LayerId(5), "Melon"));
    orig.add(std::move(std::make_unique<Layer::Layer>(LayerId(6), "Sandia")));

    EXPECT_EQ(5, orig.size());
}

TEST_F(IdentifiableUniqueTest, addAndAssignId) 
{
    auto orig(newLayers());
    auto melon = std::make_unique<Layer::Layer>("Melon");
    auto melonInGroup = orig.addAndAssignId(std::move(melon))->get();
    EXPECT_EQ(LayerId(4), melonInGroup->getId());
}

