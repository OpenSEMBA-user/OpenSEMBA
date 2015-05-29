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

TEST_F(MesherGroupPixelsConfTest, CoarseGrid) {
    init(CVecR3(0.0), CVecR3(5.0), CVecR3(2.5), coords_);
    IntersectionsGenerator generator;
    IntersectionsGroup* intersections = generator(grid, uMesh);

    GroupPixelsConf conformalPixels(*intersections);
    EXPECT_EQ(3, conformalPixels.getVoxels()->size());
    EXPECT_EQ(16, conformalPixels.getSurfels()->size());
    EXPECT_EQ(18, conformalPixels.getLinels()->size());

    delete intersections;
}

TEST_F(MesherGroupPixelsConfTest, FineGrid) {
    init(CVecR3(0.0), CVecR3(5.0), CVecR3(1.0), coords_);
    IntersectionsGenerator generator;
    IntersectionsGroup* intersections = generator(grid, uMesh);

    GroupPixelsConf conformalPixels(*intersections);
    EXPECT_EQ(13, conformalPixels.getVoxels()->size());
    EXPECT_EQ(60, conformalPixels.getSurfels()->size());

    delete intersections;
}
