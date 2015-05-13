#include "gtest/gtest.h"
#include "geometry/Box.h"

template <typename T>
class GeometryBoxTest : public ::testing::Test {

protected:

};

typedef ::testing::Types<Int, Real> MyTypes;
TYPED_TEST_CASE(GeometryBoxTest, MyTypes);

TYPED_TEST(GeometryBoxTest, VolumeBoxBasicOperations) {
    CartesianVector<TypeParam,3> min(-0.1, -0.1, -0.1);
    CartesianVector<TypeParam,3> max(1.0, 1.0, 1.0);
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
    EXPECT_EQ(box.chop().size(), 64);
    EXPECT_EQ(box.chop()[0], BoxI3(CVecI3(0,0,0), CVecI3(1,1,1)));
    EXPECT_EQ(box.chop(2).size(), 8);
    EXPECT_EQ(box.chop(3).size(), 1);
    EXPECT_EQ(box.chop(4).size(), 1);
    EXPECT_EQ(box.chop(5).size(), 1);
    EXPECT_EQ(box.chop(5)[0], box);
}

TYPED_TEST(GeometryBoxTest, SurfaceBoxChop) {
    CVecI3 min(0, 0, 0);
    CVecI3 max(0, 4, 4);
    BoxI3 box(min,max);
    EXPECT_TRUE(box.isSurface());
    EXPECT_EQ(box.chop().size(), 16);
    EXPECT_EQ(box.chop(2).size(), 4);
}

