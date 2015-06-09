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
    EXPECT_EQ(added.add(group).size(), group.size());
}

TEST_F(MesherGroupCVecI3UnfinishedTest, GetVoxels) {
    GroupCVecI3FracU group;
    group.add(fracPosWithElems_);
    group.add(CVecI3FracU(CVecI3(4), CVecR3(0.0)));
    group.add(CVecI3FracU(CVecI3(1,2,3), CVecR3(0.0)));
    EXPECT_EQ(3, group.getVoxels().size());
}


TEST_F(MesherGroupCVecI3UnfinishedTest, GetIntersectionsAtVoxel) {
    GroupCVecI3FracU group;
    group.add(fracPosWithElems_);
    group.add(CVecI3FracU(CVecI3(1,2,3), CVecR3(0.0)));
    GroupCVecI3FracU sameVoxel;
    CVecI3 pos(4);
    sameVoxel.add(CVecI3FracU(pos, CVecR3(0.0)));
    sameVoxel.add(CVecI3FracU(pos, CVecR3(0.32)));
    sameVoxel.add(CVecI3FracU(pos, CVecR3(0.35)));
    sameVoxel.add(CVecI3FracU(pos+CVecI3(1,0,0), CVecR3(0.0, 0.0, 0.35)));
    sameVoxel.add(CVecI3FracU(pos+1, CVecR3(0.0)));
    group.add(sameVoxel);
    GroupCVecI3FracU gettedVoxel = group.getIntersectionsAtVoxel(Voxel(pos));
    EXPECT_EQ(sameVoxel.size(), gettedVoxel.size());
    GroupCVecI3FracU::iterator it, that;
    for (it = sameVoxel.begin(); it != sameVoxel.end(); ++it) {
        that = gettedVoxel.find(*it);
        EXPECT_EQ(*(*it), *(*that));
    }
}


TEST_F(MesherGroupCVecI3UnfinishedTest, GetIntersectionsAtSurfel) {

    Int rot = 0;
    for(Int rot=0; rot<3; ++rot){
        CVecI3 origin(1,2,1);
        origin.cyclicPermutation(rot);

        Intersection intersection1_in1 (CVecI3Frac(origin + CVecI3(0,0,0).cyclicPermutation(rot),CVecR3(0.0,0.0,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection1_in2 (CVecI3Frac(origin + CVecI3(0,0,0).cyclicPermutation(rot),CVecR3(0.2,0.0,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection1_in3 (CVecI3Frac(origin + CVecI3(0,0,0).cyclicPermutation(rot),CVecR3(0.0,0.2,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection1_in4 (CVecI3Frac(origin + CVecI3(0,0,0).cyclicPermutation(rot),CVecR3(0.2,0.2,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());

        Intersection intersection2_in1 (CVecI3Frac(origin + CVecI3(1,0,0).cyclicPermutation(rot),CVecR3(0.0,0.0,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection2_in2 (CVecI3Frac(origin + CVecI3(1,0,0).cyclicPermutation(rot),CVecR3(0.0,0.2,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());

        Intersection intersection3_in1 (CVecI3Frac(origin + CVecI3(0,1,0).cyclicPermutation(rot),CVecR3(0.0,0.0,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection3_in2 (CVecI3Frac(origin + CVecI3(0,1,0).cyclicPermutation(rot),CVecR3(0.2,0.0,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());

        Intersection intersection4_in1 (CVecI3Frac(origin + CVecI3(1,1,0).cyclicPermutation(rot),CVecR3(0.0,0.0,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());


        Intersection intersection1_out1 (CVecI3Frac(origin + CVecI3(0,0,0).cyclicPermutation(rot),CVecR3(0.2,0.2,0.2).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection1_out2 (CVecI3Frac(origin + CVecI3(0,0,0).cyclicPermutation(rot),CVecR3(0.0,0.0,0.2).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection1_out3 (CVecI3Frac(origin + CVecI3(0,0,0).cyclicPermutation(rot),CVecR3(0.0,0.2,0.2).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection1_out4 (CVecI3Frac(origin + CVecI3(0,0,0).cyclicPermutation(rot),CVecR3(0.2,0.0,0.2).cyclicPermutation(rot)),
                ConstElemRGroup());

        Intersection intersection2_out1 (CVecI3Frac(origin + CVecI3(1,0,0).cyclicPermutation(rot),CVecR3(0.2,0.0,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection2_out2 (CVecI3Frac(origin + CVecI3(1,0,0).cyclicPermutation(rot),CVecR3(0.0,0.0,0.2).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection2_out3 (CVecI3Frac(origin + CVecI3(1,0,0).cyclicPermutation(rot),CVecR3(0.2,0.2,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());

        Intersection intersection3_out1 (CVecI3Frac(origin + CVecI3(0,1,0).cyclicPermutation(rot),CVecR3(0.0,0.2,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection3_out2 (CVecI3Frac(origin + CVecI3(0,1,0).cyclicPermutation(rot),CVecR3(0.0,0.0,0.2).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection3_out3 (CVecI3Frac(origin + CVecI3(0,1,0).cyclicPermutation(rot),CVecR3(0.2,0.2,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());

        Intersection intersection4_out1 (CVecI3Frac(origin + CVecI3(1,1,0).cyclicPermutation(rot),CVecR3(0.0,0.2,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection4_out2 (CVecI3Frac(origin + CVecI3(1,1,0).cyclicPermutation(rot),CVecR3(0.2,0.0,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection4_out3 (CVecI3Frac(origin + CVecI3(1,1,0).cyclicPermutation(rot),CVecR3(0.2,0.2,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());

        Intersection intersection5_out1 (CVecI3Frac(origin + CVecI3(0,0,0).cyclicPermutation(rot),CVecR3(0.0,0.0,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection5_out2 (CVecI3Frac(origin + CVecI3(0,2,0).cyclicPermutation(rot),CVecR3(0.0,0.0,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection5_out3 (CVecI3Frac(origin + CVecI3(1,2,0).cyclicPermutation(rot),CVecR3(0.0,0.0,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection5_out4 (CVecI3Frac(origin + CVecI3(2,1,0).cyclicPermutation(rot),CVecR3(0.0,0.0,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());
        Intersection intersection5_out5 (CVecI3Frac(origin + CVecI3(1,1,1).cyclicPermutation(rot),CVecR3(0.0,0.0,0.0).cyclicPermutation(rot)),
                ConstElemRGroup());

        IntersectionsGroup intersectionGroup;

        CartesianDirection normIdBase = (CartesianDirection) ((3+rot)%3);

        Surfel surfelBase(origin,normIdBase);

        EXPECT_TRUE(surfelBase.isInto(intersection1_in1));
        EXPECT_TRUE(surfelBase.isInto(intersection1_in2));
        EXPECT_TRUE(surfelBase.isInto(intersection1_in3));
        EXPECT_TRUE(surfelBase.isInto(intersection1_in4));
        EXPECT_TRUE(surfelBase.isInto(intersection2_in1));
        EXPECT_TRUE(surfelBase.isInto(intersection2_in2));
        EXPECT_TRUE(surfelBase.isInto(intersection3_in1));
        EXPECT_TRUE(surfelBase.isInto(intersection3_in2));
        EXPECT_TRUE(surfelBase.isInto(intersection4_in1));


        EXPECT_FALSE(surfelBase.isInto(intersection1_out1));
        EXPECT_FALSE(surfelBase.isInto(intersection1_out2));
        EXPECT_FALSE(surfelBase.isInto(intersection1_out3));
        EXPECT_FALSE(surfelBase.isInto(intersection1_out4));

        EXPECT_FALSE(surfelBase.isInto(intersection2_out1));
        EXPECT_FALSE(surfelBase.isInto(intersection2_out2));
        EXPECT_FALSE(surfelBase.isInto(intersection2_out3));

        EXPECT_FALSE(surfelBase.isInto(intersection3_out1));
        EXPECT_FALSE(surfelBase.isInto(intersection3_out2));
        EXPECT_FALSE(surfelBase.isInto(intersection3_out3));

        EXPECT_FALSE(surfelBase.isInto(intersection4_out1));
        EXPECT_FALSE(surfelBase.isInto(intersection4_out2));
        EXPECT_FALSE(surfelBase.isInto(intersection4_out3));

        EXPECT_FALSE(surfelBase.isInto(intersection5_out1));
        EXPECT_FALSE(surfelBase.isInto(intersection5_out2));
        EXPECT_FALSE(surfelBase.isInto(intersection5_out3));

        vector<Surfel> surfels;
        //Test getSurfels
        surfels = intersectionGroup.getSurfels();
        EXPECT_EQ(surfels.size(),0);

        //add intersection
        intersectionGroup.add (intersection1_in1);
        intersectionGroup.add (intersection1_in2);
        intersectionGroup.add (intersection1_in3);
        intersectionGroup.add (intersection1_in4);



        //Test getSurfels
        surfels = intersectionGroup.getSurfels();
        EXPECT_EQ(surfels.size(),6);

        //add intersection
        intersectionGroup.add (intersection2_in1);
        intersectionGroup.add (intersection2_in2);



        //Test getSurfels
        surfels = intersectionGroup.getSurfels();
        EXPECT_EQ(surfels.size(),10);

        //add intersection
        intersectionGroup.add (intersection3_in1);
        intersectionGroup.add (intersection3_in2);



        //Test getSurfels
        surfels = intersectionGroup.getSurfels();
        EXPECT_EQ(surfels.size(),15);

        //add intersection
        intersectionGroup.add (intersection4_in1);



        //Test getSurfels
        surfels = intersectionGroup.getSurfels();
        EXPECT_EQ(surfels.size(),20);


    }

//    intersectionGroup.add(CVecI3FracU(CVecI3(1,2,3), CVecR3(0.0)));
//
//    GroupCVecI3FracU sameVoxel;
//    CVecI3 pos(4);
//    sameVoxel.add(CVecI3FracU(pos, CVecR3(0.0)));
//    sameVoxel.add(CVecI3FracU(pos, CVecR3(0.32)));
//    sameVoxel.add(CVecI3FracU(pos, CVecR3(0.35)));
//    sameVoxel.add(CVecI3FracU(pos+CVecI3(1,0,0), CVecR3(0.0, 0.0, 0.35)));
//    sameVoxel.add(CVecI3FracU(pos+1, CVecR3(0.0)));
//    group.add(sameVoxel);
//    GroupCVecI3FracU gettedVoxel = group.getIntersectionsAtVoxel(Voxel(pos));
//    EXPECT_EQ(sameVoxel.size(), gettedVoxel.size());
//    GroupCVecI3FracU::iterator it, that;
//    for (it = sameVoxel.begin(); it != sameVoxel.end(); ++it) {
//        that = gettedVoxel.find(*it);
//        EXPECT_EQ(*(*it), *(*that));
//    }
}

