
#include "CoordinateTest.h"

using namespace std;

using namespace SEMBA;
using namespace Geometry;

TEST_F(GeometryCoordinateGroupTest, Copy){
    Coordinate::Group<>* original = new Coordinate::Group<>(buildCoordGroup());
    Coordinate::Group<> copied;
    copied = *original;

    EXPECT_TRUE(checkTypes(*original));
    EXPECT_TRUE(checkTypes(copied));

    delete original;

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
