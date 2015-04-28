#include "gtest/gtest.h"
#include "math/CartesianVector.h"

TEST(MathUtilsTest, equal) {
    vector<Real> num;
    num.push_back((Real) -2.69386e-10);
    num.push_back((Real) 1e-10);

    for (UInt i = 0; i < num.size(); i++) {
        EXPECT_TRUE(MathUtils::equal(num[i], num[i]));
    }
}
