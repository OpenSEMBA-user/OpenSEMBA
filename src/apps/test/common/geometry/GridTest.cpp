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

        vector<Real> steps[3];
        steps[x] = grid_.getStep(x);
        steps[y] = grid_.getStep(y);
        steps[z] = grid_.getStep(z);
        cartesianGrid_ = new Grid3 (CVecI3(), offsetIni, steps);
    }

    void checkNaturalCell(double pos) {
        long int ijk1, ijk2;
        double relLen1, relLen2;

        EXPECT_EQ(        grid_.getNaturalCellx(pos, ijk1, relLen1),
                cartesianGrid_->getNaturalCellx(pos, ijk2, relLen2));
        EXPECT_EQ(ijk1, ijk2);
        EXPECT_NEAR(relLen1, relLen2, 1e-5);

        EXPECT_EQ(        grid_.getNaturalCelly(pos, ijk1, relLen1),
                cartesianGrid_->getNaturalCelly(pos, ijk2, relLen2));
        EXPECT_EQ(ijk1, ijk2);
        EXPECT_NEAR(relLen1, relLen2, 1e-5);

        EXPECT_EQ(        grid_.getNaturalCellz(pos, ijk1, relLen1),
                cartesianGrid_->getNaturalCellz(pos, ijk2, relLen2));
        EXPECT_EQ(ijk1, ijk2);
        EXPECT_NEAR(relLen1, relLen2, 1e-5);
    }

    Grid3 grid_;
    Grid3* cartesianGrid_;
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

TEST_F(GeometryGridTest, GetCVecI3Fractional) {
    bool err;
    EXPECT_EQ(CVecI3Frac(CVecI3(3), CVecR3(0.02)),
               grid_.getCVecI3Fractional(CVecR3(0.17), &err));

}

TEST_F(GeometryGridTest, cartesianGridvsGrid3) {
    // Dims
    EXPECT_EQ(grid_.getDimsx(), cartesianGrid_->getDimsx());
    EXPECT_EQ(grid_.getDimsy(), cartesianGrid_->getDimsy());
    EXPECT_EQ(grid_.getDimsz(), cartesianGrid_->getDimsz());
    // Pos
    for (UInt i = 0; i < grid_.getDimsx()+1; i++) {
        EXPECT_NEAR(grid_.getPx()[i], cartesianGrid_->getPx()[i], 1e-4);
    }
    for (UInt i = 0; i < grid_.getDimsy()+1; i++) {
        EXPECT_NEAR(grid_.getPy()[i], cartesianGrid_->getPy()[i], 1e-4);
    }
    for (UInt i = 0; i < grid_.getDimsz()+1; i++) {
        EXPECT_NEAR(grid_.getPz()[i], cartesianGrid_->getPz()[i], 1e-4);
    }
    // Steps
    for (UInt i = 0; i < grid_.getDimsx(); i++) {
        EXPECT_EQ(grid_.getDx()[i], cartesianGrid_->getDx()[i]);
    }
    for (UInt i = 0; i < grid_.getDimsy(); i++) {
        EXPECT_EQ(grid_.getDy()[i], cartesianGrid_->getDy()[i]);
    }
    for (UInt i = 0; i < grid_.getDimsz(); i++) {
        EXPECT_EQ(grid_.getDz()[i], cartesianGrid_->getDz()[i]);
    }
}

TEST_F(GeometryGridTest, cartesianGridNatCells) {
    checkNaturalCell((double) 0.14);
    checkNaturalCell((double) 0.17);
    checkNaturalCell((double) 0.97);
    checkNaturalCell((double) 1.7);
    checkNaturalCell((double) -1);
}
