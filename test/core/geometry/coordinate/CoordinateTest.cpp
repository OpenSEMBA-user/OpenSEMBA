
#include "CoordinateTest.h"

using namespace std;

using namespace SEMBA;
using namespace Geometry;

TEST_F(GeometryCoordinateGroupTest, Copy){
    CoordR3Group copied;
    {
        auto original(buildCoordGroup());
        copied = original;

        EXPECT_TRUE(checkTypes(original));
        EXPECT_TRUE(checkTypes(copied));
    }
    EXPECT_TRUE(checkTypes(copied));
}

TEST_F(GeometryCoordinateGroupTest, getByPos){
    auto grp = buildCoordGroup();
    for (auto const& coord : grp) {
        Math::CVecR3 pos = coord->pos();
        const CoordR3* found = grp.getPos(pos);
        ASSERT_NE(nullptr, found);
        EXPECT_EQ(pos, found->pos());
    }
}
