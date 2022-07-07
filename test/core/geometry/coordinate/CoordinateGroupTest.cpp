#pragma once

#include "gtest/gtest.h"
#include "geometry/coordinate/Group.h"

class CoordinateGroupTest : public ::testing::Test {

protected:
    auto buildCoordGroup() 
    {
        SEMBA::Geometry::CoordId id(1);
        SEMBA::Math::CVecR3 one(1.0, 1.0, 1.0);
        const size_t nCoords = 5;
        SEMBA::Geometry::CoordR3Group res;
        for (size_t i = 0; i < nCoords; i++) {
            res.add(std::make_unique<SEMBA::Geometry::CoordR3>(id++, one * (double)i));
        }
        return res;
    }
};

using namespace SEMBA;
using namespace Geometry;
TEST_F(CoordinateGroupTest, CopyCtor)
{
    auto original(buildCoordGroup());
    auto origSize = original.size();
    auto copied(original);

    EXPECT_EQ(copied.size(), original.size());
    for (auto const& c : copied) {
        auto id = c->getId();
        EXPECT_EQ(c->pos(), original.getId(id)->pos());
    }
}

TEST_F(CoordinateGroupTest, MoveCtor)
{
    auto original(buildCoordGroup());
    auto origSize = original.size();
    
    auto moved(std::move(original));

    EXPECT_EQ(0, original.size());
    EXPECT_EQ(origSize, moved.size());
}

TEST_F(CoordinateGroupTest, CopyAssignment)
{
    auto original(buildCoordGroup());
    auto origSize = original.size();
    auto copied = original;

    EXPECT_EQ(copied.size(), original.size());
    for (auto const& c : copied) {
        auto id = c->getId();
        EXPECT_EQ(c->pos(), original.getId(id)->pos());
    }
}

TEST_F(CoordinateGroupTest, MoveAssignment)
{
    auto original(buildCoordGroup());
    auto origSize = original.size();

    auto moved = std::move(original);

    EXPECT_EQ(0, original.size());
    EXPECT_EQ(origSize, moved.size());
}

TEST_F(CoordinateGroupTest, AddAndGetPosition)
{
    auto grp(buildCoordGroup());
    
    Math::CVecR3 newPos(11.0);

    auto index = grp.getIndex<Math::CVecR3>();

    EXPECT_EQ(index.find(newPos), index.end());

    grp.addAndAssignId(std::make_unique<CoordR3>(CoordId(), newPos));

    index = grp.getIndex<Math::CVecR3>();
    auto it = index.find(newPos);
    EXPECT_FALSE(it == index.end());

    EXPECT_EQ(1, it->second.size());
    EXPECT_EQ(newPos, it->second.front()->pos());

}

TEST_F(CoordinateGroupTest, GetAllPositions) 
{
    auto grp(buildCoordGroup());
    
    auto index = grp.getIndex<Math::CVecR3>();

    for (auto const& coord : grp) {
        Math::CVecR3 pos = coord->pos();

        auto it = index.find(pos);

        ASSERT_NE(it, index.end());

        const CoordR3* found = it->second.front();

        ASSERT_NE(nullptr, found);
        EXPECT_EQ(pos, found->pos());
    }
}

TEST_F(CoordinateGroupTest, GetPositionsAfterMove)
{
    auto original(buildCoordGroup());

    auto& pos = original.getId(CoordId(1))->pos();
    
    auto index = original.getIndex<Math::CVecR3>();

    auto& coord = index.find(pos)->second;
    EXPECT_EQ(pos, coord.front()->pos());

    auto moved(std::move(original));
    EXPECT_EQ(pos, coord.front()->pos());

    index = original.getIndex<Math::CVecR3>();
    EXPECT_EQ(index.find(pos), index.end());
}


TEST_F(CoordinateGroupTest, PolymorphicAdd) 
{
    CoordGroup grp;
    grp.add(std::make_unique<CoordR3>(CoordId(1), Math::CVecR3(1.0)));
    grp.add(std::make_unique<CoordI3>(CoordId(2), Math::CVecI3(2)));

    EXPECT_EQ(2, grp.size());
}

TEST_F(CoordinateGroupTest, PolymorphicSizeOf)
{
    CoordGroup grp;
    grp.add(std::make_unique<CoordR3>(CoordId(1), Math::CVecR3(1.0)));
    grp.add(std::make_unique<CoordI3>(CoordId(2), Math::CVecI3(2)));
    grp.add(std::make_unique<CoordI3>(CoordId(3), Math::CVecI3(3)));

    EXPECT_EQ(1, grp.sizeOf<CoordR3>());
    EXPECT_EQ(2, grp.sizeOf<CoordI3>());

    CoordGroup copied = grp;
    EXPECT_EQ(1, copied.sizeOf<CoordR3>());
    EXPECT_EQ(2, copied.sizeOf<CoordI3>());
}

TEST_F(CoordinateGroupTest, AddAndAssignIdShouldUpdateEntryOnce)
{
    auto grp(buildCoordGroup());

    EXPECT_EQ(5, grp.size());

    auto index = grp.getIndex<Math::CVecR3>();

    EXPECT_EQ(index.find(Math::CVecR3(3.1416)), index.end());

    SEMBA::Geometry::CoordR3 coord(CoordId(), Math::CVecR3(3.1416));

    grp.addAndAssignId(
        std::make_unique<SEMBA::Geometry::CoordR3>(coord)
    );

    EXPECT_EQ(6, grp.size());

    index = grp.getIndex<Math::CVecR3>();

    auto it = index.find(Math::CVecR3(3.1416));
    EXPECT_FALSE(it == index.end());
    EXPECT_EQ(1, it->second.size());
}