#include "gtest/gtest.h"
#include "geometry/Grid.h"
//#include "apps/ugrMesher/cartesianGrid.h"

class GeometryGridTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        min_ = CVecR3(0.0, 0.0, 0.0);
        max_ = CVecR3(1.0, 1.0, 1.0);
        step_ = CVecR3(0.05, 0.05, 0.05);
        grid_ = Grid3(BoxR3(min_, max_), step_);


        vector<double> offset_;
        vector<double> off_;
        off_.resize(3);
        CVecR3 offsetIni;

        offsetIni = grid_.getOrigin();

    }

    Grid3 grid_;
    CVecR3 min_, max_, step_;
};

TEST_F(GeometryGridTest, BasicOperations) {
    EXPECT_EQ(grid_.getNumCells(), CVecI3(20, 20, 20));
    EXPECT_EQ(Grid3(BoxR3(min_,min_), CVecR3()).getNumCells(), CVecI3(1,1,1));
    EXPECT_EQ(Grid3(BoxR3(min_,max_), CVecR3()).getNumCells(), CVecI3(1,1,1));
}

TEST_F(GeometryGridTest, NaturalCells) {
    const double tol = step_(x) / 1.0e3;
    EXPECT_EQ(grid_.getCell(min_, false, 0.0), CVecI3(0,0,0));
    EXPECT_EQ(grid_.getCell(max_, false, tol), grid_.getNumCells());
}

TEST_F(GeometryGridTest, NumberOfCells) {
    Grid3 grid(BoxR3(min_, max_), CVecR3(0.75, 0.75, 0.75));
    EXPECT_EQ(grid.getFullDomainBoundingBox(), BoxR3(min_,CVecR3(1.5,1.5,1.5)));
    EXPECT_EQ(grid.getNumCells(), CVecI3(2,2,2));
}

TEST_F(GeometryGridTest, PosInRange) {
    vector<Real> posInRange = grid_.getPosInRange(x,0.17,0.27);
    EXPECT_EQ(2, posInRange.size());
    EXPECT_EQ(0.2, posInRange[0]);
    EXPECT_EQ(0.25, posInRange[1]);
}

TEST_F(GeometryGridTest, GetCVecI3Fractional) {
    bool err;
    EXPECT_EQ(CVecI3Frac(CVecI3(0), CVecR3(0.0)),
               grid_.getCVecI3Fractional(CVecR3(0.0), err));
    EXPECT_TRUE(err);

    EXPECT_EQ(CVecI3Frac(CVecI3(3), CVecR3(0.02/0.05)),
               grid_.getCVecI3Fractional(CVecR3(0.17), err));
    EXPECT_TRUE(err);

    EXPECT_EQ(CVecI3Frac(CVecI3(4), CVecR3(0.0)),
               grid_.getCVecI3Fractional(CVecR3(0.2), err));
    EXPECT_TRUE(err);

    grid_.getCVecI3Fractional(CVecR3(1.01), err);
    EXPECT_FALSE(err);

    grid_.getCVecI3Fractional(CVecR3(-0.01), err);
    EXPECT_FALSE(err);
}

TEST_F(GeometryGridTest, GetSteps) {
    EXPECT_NEAR(0.05, grid_.getStep(0,2),MathUtils::tolerance);
    EXPECT_NEAR(0.05, grid_.getStep(1,5),MathUtils::tolerance);
    EXPECT_NEAR(0.05, grid_.getStep(2,5),MathUtils::tolerance);
}
