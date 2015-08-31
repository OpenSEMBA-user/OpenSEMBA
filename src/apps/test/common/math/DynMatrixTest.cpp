#include "gtest/gtest.h"
#include "math/DynMatrix.h"

class MathDynMatrixTest : public ::testing::Test {

protected:
   virtual void SetUp() {
      // Inits. a matrix.
      a_ = DynMatrix<double>(2,2);
      a_(0,0) = 1.0; a_(0,1) = 2.0;
      a_(1,0) = 3.0; a_(1,1) = 5.0;
      // Inits. b_ matrix.
      b_ = DynMatrix<double>(2,2);
      b_(0,0) = 7.0;  b_(0,1) = 11.0;
      b_(1,0) = 13.0; b_(1,1) = 17.0;
   }
   DynMatrix<double> a_;
   DynMatrix<double> b_;
};

TEST_F(MathDynMatrixTest, BasicOperations) {
   DynMatrix<double> c(2,2);
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
   EXPECT_EQ(c, DynMatrix<double>(2,2));
   c += a_;
   EXPECT_EQ(c, a_);
   EXPECT_EQ(c *= 2.0, a_ * 2.0);
   // Transpose
   EXPECT_EQ(a_, a_.transpose().transpose());
   // Kronecker tensor product.
   c = a_.kron(b_);
   EXPECT_EQ(c.nRows(), a_.nRows() + b_.nRows());
   EXPECT_EQ(c.nCols(), a_.nCols() + b_.nCols());
   pair<int,int> fIndices(0,1), sIndices(2,3);
   EXPECT_EQ(c.sub(fIndices, fIndices), b_ * a_(0,0));
   EXPECT_EQ(c.sub(fIndices, sIndices), b_ * a_(0,1));
   EXPECT_EQ(c.sub(sIndices, fIndices), b_ * a_(1,0));
   EXPECT_EQ(c.sub(sIndices, sIndices), b_ * a_(1,1));
}

TEST_F(MathDynMatrixTest, Inversion) {
   // 2x2
   DynMatrix<double> c(2,2), aux(2,2);
   c(0,0) = -5.0; c(0,1) =  2.0;
   c(1,0) =  3.0; c(1,1) = -1.0;
   EXPECT_EQ(c, a_.invert());
   EXPECT_EQ(a_, a_.invert().invert());
   EXPECT_EQ(b_, b_.invert().invert());
   // 3x3
   DynMatrix<double> d(3,3);
   d(0,0) = 1.0; d(0,1) = 5.0; d(0,2) = 13.0;
   d(1,0) = 2.0; d(1,1) = 7.0; d(1,2) = 17.0;
   d(2,0) = 3.0; d(2,1) = 11.0; d(2,2) = 19.0;
   EXPECT_EQ(d, d.invert().invert());
}
