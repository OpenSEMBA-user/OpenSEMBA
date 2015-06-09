/*
 * GroupPixelsConfTest.cpp
 *
 *  Created on: May 25, 2015
 *      Author: luis
 */

#include "gtest/gtest.h"
#include "../IntersectionsGeneratorTest.h"
#include "mesher/fdtd/meshConf/pixelConf/GroupPixelsConf.h"

class MesherGroupPixelsConfTest : public MesherIntersectionsGeneratorTest {
public:
    void SetUp() {
        coords_.push_back(new CoordR3(CoordinateId(1), CVecR3(0.5, 0.5, 0.5)));
        coords_.push_back(new CoordR3(CoordinateId(2), CVecR3(3.8, 0.5, 0.5)));
        coords_.push_back(new CoordR3(CoordinateId(3), CVecR3(0.5, 3.8, 0.5)));
    }
protected:
    vector<CoordR3*> coords_;
};

TEST_F(MesherGroupPixelsConfTest, SetLinelsConfInsertion) {
    CVecI3 pos(1);
    LinelConf* linel1 = new LinelConf(Linel(pos,CartesianDirection::dirX));
    LinelConf* linel2 = new LinelConf(Linel(pos,CartesianDirection::dirY));
    LinelConf* copied = new LinelConf(*linel1);
    EXPECT_FALSE(*linel1 < *copied);

    SetLinelConf linels;
    EXPECT_TRUE(linels.insert(linel1).second);
    EXPECT_TRUE(linels.insert(linel2).second);
    EXPECT_FALSE(linels.insert(linel1).second);
    EXPECT_FALSE(linels.insert(copied).second);

    EXPECT_EQ(**linels.find(linel1), *linel1);
    EXPECT_EQ(**linels.find(copied), *linel1);
}

//
//TEST_F(MesherGroupPixelsConfTest, CoarseGrid) {
//    init(CVecR3(0.0), CVecR3(5.0), CVecR3(2.5), coords_);
//    IntersectionsGenerator generator;
//    IntersectionsGroup* intersections = generator(grid, uMesh);
//    // Counts number of elements.
//    GroupPixelsConf confixels(*intersections);
//    EXPECT_EQ(3, confixels.getVoxels()->size());
//    EXPECT_EQ(16, confixels.getSurfels()->size());
//    EXPECT_EQ(28, confixels.getLinels()->size());
//    // Checks some pixels conf.
//    // LinelsConf.
//    {
//        LinelConf lixC(Linel(CVecI3(0), CartesianDirection::dirX));
//        SetLinelConf::iterator it = confixels.getLinels()->find(&lixC);
//        EXPECT_EQ(0, (*it)->size());
//    }
//    // SurfelsConf.
//    {
//        SurfelConf suxC(Surfel(CVecI3(1,0,0), CartesianDirection::dirX));
//        SetSurfelConf::iterator it = confixels.getSurfels()->find(&suxC);
//        EXPECT_EQ(2, (*it)->size());
//    }
//    // VoxelsConf.
//    {
//        VoxelConf voxC(Voxel(CVecI3(0)));
//        SetVoxelConf::iterator it = confixels.getVoxels()->find(&voxC);
//        EXPECT_EQ(5, (*it)->size());
//    }
//    {
//        VoxelConf voxC(Voxel(CVecI3(1,0,0)));
//        SetVoxelConf::iterator it = confixels.getVoxels()->find(&voxC);
//        EXPECT_EQ(3, (*it)->size());
//    }
//    delete intersections;
//}
//
//TEST_F(MesherGroupPixelsConfTest, FineGrid) {
//    init(CVecR3(0.0), CVecR3(5.0), CVecR3(1.0), coords_);
//    IntersectionsGenerator generator;
//    IntersectionsGroup* intersections = generator(grid, uMesh);
//
//    GroupPixelsConf conformalPixels(*intersections);
//    EXPECT_EQ(13, conformalPixels.getVoxels()->size());
//    EXPECT_EQ(60, conformalPixels.getSurfels()->size());
//
//    delete intersections;
//}
