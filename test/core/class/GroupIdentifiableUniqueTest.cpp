#include "gtest/gtest.h"

#include "class/GroupIdentifiableUnique.h"
#include "geometry/layer/Layer.h"

using namespace SEMBA;
using namespace Geometry;
using namespace Class::Group;

class GroupIdentifiableUniqueTest : public ::testing::Test {
public:
    
protected:
    auto buildGroupOfThreeLayers() const 
    {
        GroupIdentifiableUnique<Layer::Layer> res;
        res.add(std::make_unique<Layer::Layer>(LayerId(1), "Patata"));
        res.add(std::make_unique<Layer::Layer>(LayerId(2), "Cebolla"));
        res.add(std::make_unique<Layer::Layer>(LayerId(3), "Huevos"));
        return res;
    }
};

TEST_F(GroupIdentifiableUniqueTest, CopyAndMoveCtor) 
{
    auto orig = buildGroupOfThreeLayers();
    std::size_t origSize = orig.size();

    auto copied( orig );
    EXPECT_EQ(origSize, copied.size());

    auto moved(std::move(orig));
    EXPECT_EQ(origSize, moved.size());
    EXPECT_EQ(origSize, copied.size());
    EXPECT_EQ(0, orig.size());
}

TEST_F(GroupIdentifiableUniqueTest, CopyAndMoveAssignment) 
{
    auto orig = buildGroupOfThreeLayers();
    std::size_t origSize = orig.size();
    
    auto copied = orig;
    EXPECT_EQ(origSize, copied.size());
    
    auto moved = std::move(orig);
    EXPECT_EQ(origSize, moved.size());
    EXPECT_EQ(origSize, copied.size());
    EXPECT_EQ(0, orig.size());
}

TEST_F(GroupIdentifiableUniqueTest, DeepCopyAndMoveAdd) 
{
    auto orig = buildGroupOfThreeLayers();
    orig.add(std::make_unique<Layer::Layer>(LayerId(5), "Melon"));
    orig.add(std::move(std::make_unique<Layer::Layer>(LayerId(6), "Sandia")));

    EXPECT_EQ(5, orig.size());
}

TEST_F(GroupIdentifiableUniqueTest, AddAndAssignId) 
{
    auto orig(buildGroupOfThreeLayers());
    auto melon = std::make_unique<Layer::Layer>("Melon");
    auto melonInGroup = orig.addAndAssignId(std::move(melon))->get();
    EXPECT_EQ(LayerId(4), melonInGroup->getId());
}

TEST_F(GroupIdentifiableUniqueTest, AddAndAssignIds) 
{
	auto orig(buildGroupOfThreeLayers());

	auto melonInGroup = orig.addAndAssignId(std::move(
		std::make_unique<Layer::Layer>("Melon")
	))->get();

	auto nisporaInGroup = orig.addAndAssignId(std::move(
		std::make_unique<Layer::Layer>("N�spora")
	))->get();

	EXPECT_EQ(LayerId(5), nisporaInGroup->getId());

	GroupIdentifiableUnique<Layer::Layer> another;
	auto naranjaInGroup = another.addAndAssignId(std::move(
		std::make_unique<Layer::Layer>("Naranja")
	))->get();

	another.addAndAssignIds(orig);

	EXPECT_EQ(LayerId(5), nisporaInGroup->getId());

	EXPECT_EQ(6, another.size());
	EXPECT_EQ("N�spora", another.getId(LayerId(6))->getName());
}

TEST_F(GroupIdentifiableUniqueTest, DuplicatedIdShouldTriggerLogicException)
{
    auto orig(buildGroupOfThreeLayers());

    EXPECT_EQ(orig.getId(LayerId(1))->getName(), "Patata");

    orig.add(std::make_unique<Layer::Layer>(LayerId(1), "Another Patata"));

    EXPECT_EQ(orig.getId(LayerId(1))->getName(), "Another_Patata");

    for (const auto& item : orig) {
        EXPECT_TRUE(item->getName() != "Patata");
    }
}
