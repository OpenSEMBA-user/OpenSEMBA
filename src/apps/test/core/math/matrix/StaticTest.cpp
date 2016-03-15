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
#include "gtest/gtest.h"
#include "math/matrix/Static.h"

using namespace SEMBA;
using namespace Math;
using namespace Matrix;

class MathMatrixStaticTest : public ::testing::Test {

protected:
	virtual void SetUp() {
		// Inits. a matrix.
		a_(0,0) = 1.0; a_(0,1) = 2.0;
		a_(1,0) = 3.0; a_(1,1) = 5.0;
		// Inits. b_ matrix.
		b_(0,0) = 7.0;  b_(0,1) = 11.0;
		b_(1,0) = 13.0; b_(1,1) = 17.0;

		// Inits complex a matrix
		complex_a_(0,0) = std::complex<double>(0.0,1.0); complex_a_(0,1) = std::complex<double>(0.0,0.0);
		complex_a_(1,0) = std::complex<double>(0.0,0.0); complex_a_(1,1) = std::complex<double>(1.0,0.0);
	}
	Static<double,2,2> a_;
	Static<double,2,2> b_;

	// Complex test matrices
	MatC22 complex_a_;
};

TEST_F(MathMatrixStaticTest, BasicOperations) {
	Static<double,2,2> c;
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
	Static<double,2,2> def;
	EXPECT_EQ(c, def);
	c += a_;
	EXPECT_EQ(c, a_);
	EXPECT_EQ(c *= 2.0, a_ * 2.0);
	// Transpose
	EXPECT_EQ(a_, a_.transpose().transpose());
}

TEST_F(MathMatrixStaticTest, Inversion) {
	// 2x2
	Static<double,2,2> c;
	c(0,0) = -5.0; c(0,1) =  2.0;
	c(1,0) =  3.0; c(1,1) = -1.0;
	EXPECT_EQ(c, a_.invert());
	EXPECT_EQ(a_, a_.invert().invert());
	EXPECT_EQ(b_, b_.invert().invert());
	// 3x3
	Static<double,3,3> d;
	d(0,0) = 1.0; d(0,1) = 5.0; d(0,2) = 13.0;
	d(1,0) = 2.0; d(1,1) = 7.0; d(1,2) = 17.0;
	d(2,0) = 3.0; d(2,1) = 11.0; d(2,2) = 19.0;
	EXPECT_EQ(d, d.invert().invert());
}

TEST_F(MathMatrixStaticTest, complexInversion) {
	// 2x2
	MatC22 c;
	c(0,0) = std::complex<double>(0.0,-1.0); c(0,1) = std::complex<double>(0.0,0.0);
	c(1,0) = std::complex<double>(0.0,0.0);  c(1,1) = std::complex<double>(1.0,0.0);
	EXPECT_EQ(c, c.invert().invert());
	EXPECT_EQ(c, complex_a_.invert());

	// 3x3
	MatC33 d;
	d(0,0) = std::complex<double>(0.0005,-1.0); d(0,1) = std::complex<double>(999.0,0.0);   d(0,2) = std::complex<double>(0.0,0.0);
	d(1,0) = std::complex<double>(0.0,15648.0); d(1,1) = std::complex<double>(1.0,0.00001); d(1,2) = std::complex<double>(5.0,0.0);
	d(2,0) = std::complex<double>(50.0,-50.0);  d(2,1) = std::complex<double>(1.0,1.0);     d(2,2) = std::complex<double>(1.0,1.0);
	EXPECT_EQ(d, d.invert().invert());
}
