

#include "BoxTest.h"

using namespace SEMBA;
using namespace Math;
using namespace Geometry;

typedef ::testing::Types<Int, Real> MyTypes;
TYPED_TEST_CASE(GeometryBoxTest, MyTypes);

TYPED_TEST(GeometryBoxTest, VolumeBoxBasicOperations) {
    Vector::Cartesian<TypeParam,3> min(-0.1, -0.1, -0.1);
    Vector::Cartesian<TypeParam,3> max(1.0, 1.0, 1.0);
    Box<TypeParam,3> box(min, max);
    EXPECT_EQ(box.getMin(), min);
    EXPECT_EQ(box.getMax(), max);
    EXPECT_EQ(box, box);
    EXPECT_FALSE(box < box);
    EXPECT_FALSE(box > box);
    EXPECT_TRUE(box <= box);
    EXPECT_TRUE(box.isVolume());
}

TYPED_TEST(GeometryBoxTest, SurfaceBoxBasicOperations) {
    CVecR3 min(-0.1, -0.1, 1.0);
    CVecR3 max(1.0,   1.0, 1.0);
    BoxR3 box(min, max);
    EXPECT_EQ(box.getMin(), min);
    EXPECT_EQ(box.getMax(), max);
    EXPECT_TRUE(box.isSurface());
    EXPECT_EQ(box, box);
    EXPECT_FALSE(box < box);
    EXPECT_FALSE(box > box);
    EXPECT_TRUE(box <= box);
}

TYPED_TEST(GeometryBoxTest, VolumeBoxChop) {
    CVecI3 min(0, 0, 0);
    CVecI3 max(4, 4, 4);
    BoxI3 box(min,max);
    EXPECT_TRUE(box.isVolume());
    EXPECT_EQ(64, box.chop().size());
    EXPECT_EQ(BoxI3(CVecI3(0,0,0), CVecI3(1,1,1)), box.chop()[0]);
    EXPECT_EQ(8, box.chop(2).size());
    EXPECT_EQ(8, box.chop(3).size());
    EXPECT_EQ(1, box.chop(4).size());
    EXPECT_EQ(1, box.chop(5).size());
    EXPECT_EQ(box, box.chop(5)[0]);
}

TYPED_TEST(GeometryBoxTest, SurfaceBoxChop) {
    CVecI3 min(0, 0, 0);
    CVecI3 max(0, 4, 4);
    BoxI3 box(min,max);
    EXPECT_TRUE(box.isSurface());
    EXPECT_EQ(16, box.chop().size());
    EXPECT_EQ(4, box.chop(2).size());
}

TYPED_TEST(GeometryBoxTest, BoxChopGrid) {
    CVecR3 min(0.0);
    CVecR3 max(3.0);
    CVecR3 step(1.0);
    BoxR3 box(min,max);
    Grid<3> grid(box, step);

    {
        EXPECT_EQ(27, box.chop(grid).size());
    }
    {
        BoxR3 smallMinBox(min, CVecR3(2.0));
        EXPECT_EQ(8, smallMinBox.chop(grid).size());
    }
    {
        BoxR3 smallMinBox(min, CVecR3(1.5));
        EXPECT_EQ(8, smallMinBox.chop(grid).size());
    }

    {
        BoxR3 smallMaxBox(CVecR3(1.0), max);
        EXPECT_EQ(8, smallMaxBox.chop(grid).size());
    }
    {
        BoxR3 smallMaxBox(CVecR3(1.5), max);
        EXPECT_EQ(8, smallMaxBox.chop(grid).size());
    }

    {
        BoxR3 surfBox(min, min+CVecR3(3.0, 3.0, 0.0));
        EXPECT_EQ(9, surfBox.chop(grid).size());
    }
}
