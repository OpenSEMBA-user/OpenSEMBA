#include "gtest/gtest.h"
#include "geometry/pixels/Surfel.h"

class GeometrySurfelTest : public ::testing::Test {
};

TEST_F(GeometrySurfelTest, IsInto) {
    CVecI3 pos(1);
    Surfel surf(pos, CartesianDirection::dirX);

    CVecI3Frac sidePos(pos + CVecI3(1,0,0), CVecR3(0.0, 0.5, 0.0));
    EXPECT_TRUE(surf.isInto(sidePos)) <<
            "CVecI3Frac " << sidePos << " should be into surfel " << surf;
}

TEST_F(GeometrySurfelTest, Comparison) {
    Surfel surf1(CVecI3(1), CartesianDirection::dirX);
    Surfel surf2(CVecI3(1), CartesianDirection::dirY);
    EXPECT_LE(surf1, surf2);
    EXPECT_FALSE(surf1 < surf1);
}

TEST_F(GeometrySurfelTest, GetLinels) {
    CVecI3 pos(1);
    {
        Surfel surf(pos, CartesianDirection::dirX);
        SurfelLinels linels = surf.getLinels();
        EXPECT_EQ(Linel(pos, CartesianDirection::dirY), linels[0]);
        EXPECT_EQ(Linel(pos, CartesianDirection::dirZ), linels[1]);
        EXPECT_EQ(Linel(pos+CVecI3(0,0,1), CartesianDirection::dirY), linels[2]);
        EXPECT_EQ(Linel(pos+CVecI3(0,1,0), CartesianDirection::dirZ), linels[3]);
    }

    {
        Surfel surf(pos, CartesianDirection::dirY);
        SurfelLinels linels = surf.getLinels();
        EXPECT_EQ(Linel(pos, CartesianDirection::dirZ), linels[0]);
        EXPECT_EQ(Linel(pos, CartesianDirection::dirX), linels[1]);
        EXPECT_EQ(Linel(pos+CVecI3(1,0,0), CartesianDirection::dirZ), linels[2]);
        EXPECT_EQ(Linel(pos+CVecI3(0,0,1), CartesianDirection::dirX), linels[3]);
    }

    {
        Surfel surf(pos, CartesianDirection::dirZ);
        SurfelLinels linels = surf.getLinels();
        EXPECT_EQ(Linel(pos, CartesianDirection::dirX), linels[0]);
        EXPECT_EQ(Linel(pos, CartesianDirection::dirY), linels[1]);
        EXPECT_EQ(Linel(pos+CVecI3(0,1,0), CartesianDirection::dirX), linels[2]);
        EXPECT_EQ(Linel(pos+CVecI3(1,0,0), CartesianDirection::dirY), linels[3]);
    }
}
