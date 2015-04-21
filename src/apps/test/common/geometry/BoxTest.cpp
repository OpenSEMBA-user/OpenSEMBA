#include "gtest/gtest.h"
#include "geometry/Grid.h"

class GeometryBoxTest : public ::testing::Test {

protected:
   virtual void SetUp() {
      min_ = CVecR3(0.0, 0.0, 0.0);
      max_ = CVecR3(1.0, 1.0, 1.0);
   }

   CVecR3 min_, max_;
};

TEST_F(GeometryBoxTest, BasicOperations) {
   BoxR3 box(min_, max_);
   EXPECT_EQ(box, box);
   EXPECT_FALSE(box < box);
   EXPECT_FALSE(box > box);
   EXPECT_TRUE(box <= box);
}
