#include "gtest/gtest.h"
#include "geometry/Grid.h"

class GeometryGridTest : public ::testing::Test {

protected:
   virtual void SetUp() {
      min_ = CVecR3(0.0, 0.0, 0.0);
      max_ = CVecR3(1.0, 1.0, 1.0);
      step_ = CVecR3(0.05, 0.05, 0.05);
      grid_ = Grid3(BoxR3(min_, max_), step_);
   }

   Grid3 grid_;
   CVecR3 min_, max_, step_;
};

TEST_F(GeometryGridTest, BasicOperations) {
   EXPECT_EQ(grid_.getNumCells(), CVecI3(20, 20, 20));
}

TEST_F(GeometryGridTest, NaturalCells) {
   const double tol = step_(x) / 1.0e3;
   EXPECT_EQ(grid_.getCell(min_, false, 0.0), CVecI3(0,0,0));
   EXPECT_EQ(grid_.getCell(max_, false, tol), grid_.getNumCells());
}


TEST_F(GeometryGridTest, NumberOfCells) {
    Grid3 grid(BoxR3(min_, max_), CVecR3(0.75, 0.75, 0.75));
    EXPECT_EQ(grid.getNumCells(), CVecI3(2,2,2));
}
