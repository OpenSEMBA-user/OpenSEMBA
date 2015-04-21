#include "gtest/gtest.h"
#include "math/CartesianVector.h"

TEST(MathCartesianVectorTest, Basic) {
   EXPECT_EQ(CVecR3(2.0, 3.0, 4.0), CVecR3(2.0, 3.0, 4.0));
   EXPECT_NE(CVecR3(2.0, 3.0, 4.0), CVecR3(1.0, 3.0, 4.0));
   EXPECT_EQ(sqrt(4.0 + 9.0 + 16.0), CVecR3(2.0, 3.0, 4.0).norm());
   EXPECT_EQ(CVecR3(0.0, 0.0, 1.0), CVecR3(1.0,0.0,0.0) ^ CVecR3(0.0,1.0,0.0));
}

TEST(MathCartesianVectorTest, Complex) {
   complex<double> zero(0.0, 0.0);
   complex<double> realOne(1.0, 0.0);
   CVecC3 a(realOne, realOne, realOne);
   EXPECT_EQ(a, a);
   EXPECT_EQ(sqrt(3.0), a.norm());
}
