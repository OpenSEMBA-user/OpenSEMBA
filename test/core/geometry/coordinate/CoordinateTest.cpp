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
        EXPECT_EQ(c->pos(), (*original.getId(id))->pos());
    }
}

TEST_F(CoordinateGroupTest, move_ctor)
{
    auto original(buildCoordGroup());

    auto pos
    auto id = original.getPos()

    EXPECT_EQ()

    auto origSize = original.size();
    auto moved(std::move(original));

    EXPECT_EQ(0, original.size());
    EXPECT_EQ(origSize, moved.size());
}

TEST_F(CoordinateGroupTest, getByPos)
{
    auto grp = buildCoordGroup();
    for (auto const& coord : grp) {
        Math::CVecR3 pos = coord->pos();
        const CoordR3* found = grp.getPos(pos);
        ASSERT_NE(nullptr, found);
        EXPECT_EQ(pos, found->pos());
    }
}
