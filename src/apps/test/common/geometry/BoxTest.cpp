#include "gtest/gtest.h"
#include "geometry/Grid.h"

class GeometryBoxTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        min_ = CVecR3(-0.1, -0.1, -0.1);
        max_ = CVecR3(1.0, 1.0, 1.0);
    }

    CVecR3 min_, max_;
};

TEST_F(GeometryBoxTest, BasicOperations) {
    BoxR3 box(min_, max_);
    EXPECT_EQ(box.getMin(), min_);
    EXPECT_EQ(box.getMax(), max_);
    EXPECT_EQ(box, box);
    EXPECT_FALSE(box < box);
    EXPECT_FALSE(box > box);
    EXPECT_TRUE(box <= box);
    EXPECT_TRUE(box.isVolume());
}


TEST_F(GeometryBoxTest, SurfaceBox) {
    CVecR3 max = max_;
    max(z) = min_(z);
    BoxR3 box(min_, max);
    EXPECT_EQ(box.getMin(), min_);
    EXPECT_EQ(box.getMax(), max);
    EXPECT_TRUE(box.isSurface());
    EXPECT_EQ(box, box);
    EXPECT_FALSE(box < box);
    EXPECT_FALSE(box > box);
    EXPECT_TRUE(box <= box);
}
