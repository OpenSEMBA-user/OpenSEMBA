
#include "gtest/gtest.h"
#include "math/axis/Local.h"
#include "math/Constants.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace SEMBA;
using namespace Math;
using namespace Axis;

class MathAxisLocalTest : public ::testing::Test {

};

TEST_F(MathAxisLocalTest, BasicIdentities) {

    Local global(CVecR3(0.0));

    CVecR3 localOrigin(3.0,0.0,0.0);
    Local local(CVecR3(Constants::pi, Constants::pi_2, Constants::pi / 3.0), localOrigin);

    MatR33 eye;
    eye.eye();

    EXPECT_EQ(eye, global.getTransformationMatrix());

    MatR33 localTransf = local.getTransformationMatrix();
    MatR33 localTransfTransposed = local.getTransformationMatrix().transpose();
    EXPECT_EQ(eye, localTransf * localTransfTransposed);

    EXPECT_EQ(localOrigin, local.convertToGlobal(CVecR3(0.0)));
}

TEST_F(MathAxisLocalTest, VectorTransformation) {
    Local global(CVecR3(0.0));
    Local local(CVecR3(-Constants::pi/2, 0.0, 0.0));

    CVecR3 localV(1.0,1.0,0.0);
    EXPECT_EQ(CVecR3(-1.0,1.0,0.0), local.convertToGlobal(localV));
}

TEST_F(MathAxisLocalTest, TensorTransformation) {
    Local global(CVecR3(0.0));

    const Int rotations = 3;
    Local local(CVecR3(Constants::pi*2.0/((Real)rotations), 0.0, 0.0));

    MatR33 original;

    original(0,0) = 1;
    original(0,1) = 2;
    original(0,2) = 3;

    original(1,0) = 5;
    original(1,1) = 7;
    original(1,2) = 11;

    original(2,0) = 13;
    original(2,1) = 17;
    original(2,2) = 19;

    MatR33 rotated = original;
    for (Int i = 0; i < rotations; i++) {
        rotated = local.convertToGlobal(rotated);
    }
    EXPECT_EQ(original, rotated);
}
