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
TEST_F(CoordinateGroupTest, copy_ctor)
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

TEST_F(CoordinateGroupTest, move_ctor)
{
    auto original(buildCoordGroup());
    auto origSize = original.size();
    
    auto moved(std::move(original));

    EXPECT_EQ(0, original.size());
    EXPECT_EQ(origSize, moved.size());
}

TEST_F(CoordinateGroupTest, copy_assignment)
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

TEST_F(CoordinateGroupTest, move_assignment)
{
    auto original(buildCoordGroup());
    auto origSize = original.size();

    auto moved = std::move(original);

    EXPECT_EQ(0, original.size());
    EXPECT_EQ(origSize, moved.size());
}

TEST_F(CoordinateGroupTest, add_and_get_position)
{
    auto grp(buildCoordGroup());
    
    Math::CVecR3 newPos(11.0);
    EXPECT_EQ(nullptr, grp.getPos(newPos));

    grp.addPos(newPos);
    EXPECT_EQ(newPos, grp.getPos(newPos)->pos());

}

TEST_F(CoordinateGroupTest, get_all_positions) 
{
    auto grp(buildCoordGroup());
    
    for (auto const& coord : grp) {
        Math::CVecR3 pos = coord->pos();
        const CoordR3* found = grp.getPos(pos);
        ASSERT_NE(nullptr, found);
        EXPECT_EQ(pos, found->pos());
    }
}

TEST_F(CoordinateGroupTest, get_positions_after_move)
{
    auto original(buildCoordGroup());

    auto pos = original.getId(CoordId(1))->pos();
    
    auto coord = original.getPos(pos);
    EXPECT_EQ(pos, coord->pos());

    auto moved(std::move(original));
    EXPECT_EQ(pos, coord->pos());

    EXPECT_EQ(nullptr, original.getPos(pos));
}

TEST_F(CoordinateGroupTest, polymorphic_add) 
{
    CoordGroup grp;
    grp.add(std::make_unique<CoordR3>(CoordId(1), Math::CVecR3(1.0)));
    grp.add(std::make_unique<CoordI3>(CoordId(2), Math::CVecI3(2)));

    EXPECT_EQ(2, grp.size());
}

TEST_F(CoordinateGroupTest, polymorphic_sizeOf)
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