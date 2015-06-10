#include "gtest/gtest.h"
#include "math/StaMatrix.h"

class MathStaMatrixTest : public ::testing::Test {

protected:
   virtual void SetUp() {
      // Inits. a matrix.
      a_(0,0) = 1.0; a_(0,1) = 2.0;
      a_(1,0) = 3.0; a_(1,1) = 5.0;
      // Inits. b_ matrix.
      b_(0,0) = 7.0;  b_(0,1) = 11.0;
      b_(1,0) = 13.0; b_(1,1) = 17.0;
   }
   StaMatrix<double,2,2> a_;
   StaMatrix<double,2,2> b_;
};

TEST_F(MathStaMatrixTest, BasicOperations) {
   StaMatrix<double,2,2> c;
   // Addition
   c(0,0) = 8.0; c(0,1) = 13.0;
   c(1,0) = 16.0; c(1,1) = 22.0;
   EXPECT_EQ(c, a_ + b_);
   // Product
   c(0,0) = 33.0; c(0,1) = 45.0;
   c(1,0) = 86.0; c(1,1) = 118.0;
   EXPECT_EQ(c, a_ * b_);
   // Operator+=
   c.zeros();
   StaMatrix<double,2,2> def;
   EXPECT_EQ(c, def);
   c += a_;
   EXPECT_EQ(c, a_);
   EXPECT_EQ(c *= 2.0, a_ * 2.0);
   // Transpose
   EXPECT_EQ(a_, a_.transpose().transpose());
}

TEST_F(MathStaMatrixTest, Inversion) {
   // 2x2
   StaMatrix<double,2,2> c;
   c(0,0) = -5.0; c(0,1) =  2.0;
   c(1,0) =  3.0; c(1,1) = -1.0;
   EXPECT_EQ(c, a_.invert());
   EXPECT_EQ(a_, a_.invert().invert());
   EXPECT_EQ(b_, b_.invert().invert());
   // 3x3
   StaMatrix<double,3,3> d;
   d(0,0) = 1.0; d(0,1) = 5.0; d(0,2) = 13.0;
   d(1,0) = 2.0; d(1,1) = 7.0; d(1,2) = 17.0;
   d(2,0) = 3.0; d(2,1) = 11.0; d(2,2) = 19.0;
   EXPECT_EQ(d, d.invert().invert());
}
