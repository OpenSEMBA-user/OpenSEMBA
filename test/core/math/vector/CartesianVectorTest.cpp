
#include "gtest/gtest.h"
#include "math/vector/Cartesian.h"

using namespace SEMBA;
using namespace Math;
using namespace Vector;

TEST(MathCartesianVectorTest, Basic) {
    EXPECT_EQ(CVecR3(2.0, 3.0, 4.0), CVecR3(2.0, 3.0, 4.0));
    EXPECT_NE(CVecR3(2.0, 3.0, 4.0), CVecR3(1.0, 3.0, 4.0));
    EXPECT_EQ(sqrt(4.0 + 9.0 + 16.0), CVecR3(2.0, 3.0, 4.0).norm());
    EXPECT_EQ(CVecR3(0.0, 0.0, 1.0), CVecR3(1.0,0.0,0.0) ^ CVecR3(0.0,1.0,0.0));
}

TEST(MathCartesianVectorTest, InitializerList) {
    CVecR3 a = {1.0, 2.0, 3.0};
    EXPECT_EQ(a, a);
    EXPECT_EQ(sqrt(14.0), a.norm());
}

TEST(MathCartesianVectorTest, Complex) {
    std::complex<double> zero(0.0, 0.0);
    std::complex<double> realOne(1.0, 0.0);
    CVecC3 a(realOne, realOne, realOne);
    EXPECT_EQ(a, a);
    EXPECT_EQ(sqrt(3.0), a.norm());
}

TEST(MathCartesianVectorTest, BinaryConversion) {
    for (UInt i = 0; i < 8; i++) {
        CVecI3 result((i/4)%2, (i/2)%2, i%2);
        CVecI3 converted;
        EXPECT_EQ(result, converted.setAsBinary(i));
    }
}
