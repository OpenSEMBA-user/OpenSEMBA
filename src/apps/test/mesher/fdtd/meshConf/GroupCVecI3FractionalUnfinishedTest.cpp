#include "GroupCVecI3FractionalUnfinishedTest.h"


void MesherGroupCVecI3UnfinishedTest::SetUp() {
    MesherCVecI3FractionalUnfinishedTest::init();
}

TEST_F(MesherGroupCVecI3UnfinishedTest, BasicOperations){
    GroupCVecI3FracU group;
    // Checks that a coordinate is added correctly.
    CVecI3FracU* addedPos;
    addedPos = group.add(fracPosEmpty_);
    EXPECT_EQ(fracPosEmpty_, *addedPos);
    EXPECT_EQ(1, group.size());
    // Checks that add with same position is merged into a single one.
    group.add(fracPosWithElems_);
    EXPECT_EQ(1, group.size());
    EXPECT_EQ(fracPosWithElems_, *(*group.begin()));
    // Adds new position if different.
    CVecI3FracU newPos(CVecI3(4), CVecR3(0.0), eG_);
    group.add(newPos);
    EXPECT_EQ(2, group.size());

    // Ctors and assignation.
    GroupCVecI3FracU constructed(group);
    EXPECT_EQ(group, constructed);
    GroupCVecI3FracU assigned;
    assigned = group;
    EXPECT_EQ(group, constructed);

    // Group add to itself remains equal.
    GroupCVecI3FracU added;
    EXPECT_EQ(added.add(group), group);
}


TEST_F(MesherGroupCVecI3UnfinishedTest, GetVoxels) {
    GroupCVecI3FracU group;
    group.add(fracPosWithElems_);
    group.add(CVecI3FracU(CVecI3(4), CVecR3(0.0)));
    group.add(CVecI3FracU(CVecI3(1,2,3), CVecR3(0.0)));
    EXPECT_EQ(3, group.getVoxels().size());
}
