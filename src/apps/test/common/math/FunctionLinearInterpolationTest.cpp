#include "gtest/gtest.h"
#include "math/FunctionLinearInterpolation.h"

TEST(MathFuncitonLinearInterpolationTest, Basic) {
    vector<pair<Real,Real>> xy;
    xy.push_back(pair<Real,Real>(0.0,  0.0));
    xy.push_back(pair<Real,Real>(1.0, 10.0));
    LinearInterpolation linear(xy);

    EXPECT_EQ(0.0,  linear(-1.0));
    EXPECT_EQ(0.0,  linear(0.0));
    EXPECT_EQ(5.0,  linear(0.5));
    EXPECT_EQ(10.0, linear(1.0));
    EXPECT_EQ(10.0, linear(2.0));

}
