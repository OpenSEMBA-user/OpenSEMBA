// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#include <utility>

#ifdef EIGEN_SUPPORT
#include <eigen3/Eigen/Dense>
#endif

#include "gtest/gtest.h"
#include "math/matrix/Dynamic.h"


using namespace SEMBA;
using namespace Math;
using namespace Matrix;

class MathMatrixDynamicTest : public ::testing::Test {

protected:
   virtual void SetUp() {
      // Inits. a matrix.
      a_ = Dynamic<double>(2,2);
      a_(0,0) = 1.0; a_(0,1) = 2.0;
      a_(1,0) = 3.0; a_(1,1) = 5.0;
      // Inits. b_ matrix.
      b_ = Dynamic<double>(2,2);
      b_(0,0) = 7.0;  b_(0,1) = 11.0;
      b_(1,0) = 13.0; b_(1,1) = 17.0;
   }
   Dynamic<double> a_;
   Dynamic<double> b_;
};

TEST_F(MathMatrixDynamicTest, BasicOperations) {
   Dynamic<double> c(2,2);
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
   EXPECT_EQ(c, Dynamic<double>(2,2));
   c += a_;
   EXPECT_EQ(c, a_);
   EXPECT_EQ(c *= 2.0, a_ * 2.0);
   // Transpose
   EXPECT_EQ(a_, a_.transpose().transpose());
   // Kronecker tensor product.
   c = a_.kron(b_);
   EXPECT_EQ(c.nRows(), a_.nRows() + b_.nRows());
   EXPECT_EQ(c.nCols(), a_.nCols() + b_.nCols());
   std::pair<size_t,size_t> fIndices(0,1), sIndices(2,3);
   EXPECT_EQ(c.sub(fIndices, fIndices), b_ * a_(0,0));
   EXPECT_EQ(c.sub(fIndices, sIndices), b_ * a_(0,1));
   EXPECT_EQ(c.sub(sIndices, fIndices), b_ * a_(1,0));
   EXPECT_EQ(c.sub(sIndices, sIndices), b_ * a_(1,1));
}

TEST_F(MathMatrixDynamicTest, Inversion) {
   // 2x2
   Dynamic<double> c(2,2), aux(2,2);
   c(0,0) = -5.0; c(0,1) =  2.0;
   c(1,0) =  3.0; c(1,1) = -1.0;
   EXPECT_EQ(c, a_.invert());
   EXPECT_EQ(a_, a_.invert().invert());
   EXPECT_EQ(b_, b_.invert().invert());
   // 3x3
   Dynamic<double> d(3,3);
   d(0,0) = 1.0; d(0,1) = 5.0; d(0,2) = 13.0;
   d(1,0) = 2.0; d(1,1) = 7.0; d(1,2) = 17.0;
   d(2,0) = 3.0; d(2,1) = 11.0; d(2,2) = 19.0;
   EXPECT_EQ(d, d.invert().invert());
}

TEST_F(MathMatrixDynamicTest, QRdecomposition) {
    Dynamic<double> d(3,3);
    d(0,0) = 1.0; d(0,1) = 1.0; d(0,2) = 0.0;
    d(1,0) = 1.0; d(1,1) = 0.0; d(1,2) = 1.0;
    d(2,0) = 0.0; d(2,1) = 1.0; d(2,2) = 1.0;

    Dynamic<double> Q(3,3);
    Dynamic<double> R(3,3);

    d.factorizeQR_(Q, R);
    EXPECT_EQ(Q*R, d);
}

#ifdef EIGEN_SUPPORT
TEST_F(MathMatrixDynamicTest, eigenValues) {
    Dynamic<double> d(2,2);
    d(0,0) = 3.0; d(0,1) = -2.0;
    d(1,0) = 4.0; d(1,1) = -1.0;

    std::vector<std::complex<double>> eigenValues = d.eigenValues();

    EXPECT_NEAR(eigenValues[0].real(), 1.0, 1e-10);
    EXPECT_NEAR(eigenValues[0].imag(), 2.0, 1e-10);

    EXPECT_NEAR(eigenValues[1].real(), 1.0, 1e-10);
    EXPECT_NEAR(eigenValues[1].imag(), -2.0, 1e-10);
}

TEST_F(MathMatrixDynamicTest, eigenVectors) {
    Dynamic<double> d(2,2);
    d(0,0) = 3.0; d(0,1) = -2.0;
    d(1,0) = 4.0; d(1,1) = -1.0;

    // Compute the eigen values
    std::vector<std::complex<double>> eigenValues = d.eigenValues();

    // Store them as a diagonal matrix
    Dynamic <std::complex<double>> eigValDiag(2,2);
    eigValDiag(0,0) = eigenValues[0];
    eigValDiag(0,1) = std::complex<double>(0.0, 0.0);
    eigValDiag(1,0) = std::complex<double>(0.0, 0.0);
    eigValDiag(1,1) = eigenValues[1];

    // Compute the eigenvectors
    Dynamic<std::complex<double>> eigVec = d.eigenVectors();
    Dynamic<std::complex<double>> eigVecInv = eigVec.invert();

    // The eigendecomposition has to verify D = PLP^{-1}, where D
    // is the original matrix, L the diagonal matrix whose entries are
    // the eigen values and P the matrix whose columns are the eigenvectors
    Dynamic<std::complex<double>> eigenDecomp = eigVec * eigValDiag * eigVecInv;

    for (size_t i = 0; i < d.nRows(); i++) {
        for (size_t j = 0; j < d.nCols(); j++) {
            std::complex<double> val = eigenDecomp(i,j);
            EXPECT_NEAR(val.real(), d(i,j), 1e-10);
            EXPECT_NEAR(val.imag(), 0, 1e-10);
        }
    }
}
#endif
