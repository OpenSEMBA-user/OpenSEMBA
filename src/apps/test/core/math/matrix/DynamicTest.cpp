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

TEST_F(MathMatrixDynamicTest, HessenbergForm) {
    Dynamic<double> d(3,3);
    d(0,0) = 1.0; d(0,1) = 5.0; d(0,2) = 13.0;
    d(1,0) = 2.0; d(1,1) = 7.0; d(1,2) = 17.0;
    d(2,0) = 3.0; d(2,1) = 11.0; d(2,2) = 19.0;

    //d.printInfo();

    Dynamic<double> H = d.hessenbergForm();

    for (size_t i = 0; i < H.nRows(); i++) {
        for (size_t j = 0; j < i; j++) {
            EXPECT_EQ(0.0, H.val(i,j));
        }
    }

    //H.printInfo();
}

#ifdef EIGEN_SUPPORT
TEST_F(MathMatrixDynamicTest, eigenLibraryPOC) {
	Eigen::Matrix2d A;

	A <<	3, -2,
			4, -1;

	Eigen::ComplexEigenSolver<Eigen::Matrix2d> eigenSolver;
	eigenSolver.compute(A);

    EXPECT_NEAR(eigenSolver.eigenvalues()[0].real(), 1.0, 1e-10);
    EXPECT_NEAR(eigenSolver.eigenvalues()[0].imag(), -2.0, 1e-10);

    EXPECT_NEAR(eigenSolver.eigenvalues()[1].real(), 1.0, 1e-10);
    EXPECT_NEAR(eigenSolver.eigenvalues()[1].imag(), 2.0, 1e-10);

	Eigen::Matrix2cd actual_A = eigenSolver.eigenvectors() * eigenSolver.eigenvalues().asDiagonal() * eigenSolver.eigenvectors().inverse();

    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            std::complex<double> actual_elem = actual_A(i, j);
            std::complex<double> expect_elem = A(i, j);

            EXPECT_NEAR(actual_elem.real(), expect_elem.real(), 1e-10);
            EXPECT_NEAR(actual_elem.imag(), expect_elem.imag(), 1e-10);
        }
    }
	// EXPECT_EQ(A, computedA);
}

#endif
