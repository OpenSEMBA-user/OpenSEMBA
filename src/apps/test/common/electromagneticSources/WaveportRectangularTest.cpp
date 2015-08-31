/*
 * WaveportRectangularTest.cpp
 *
 *  Created on: May 4, 2015
 *      Author: luis
 */

#include "../../../../common/sources/WaveportRectangular.h"

#include "gtest/gtest.h"

class EMSourceWaveportRectangularTest : public ::testing::Test {
    void SetUp() {
        BoxI3 plane(CVecI3(0,0,0), CVecI3(30,10,0));
        vector<BoxI3> quadBoxes = plane.chop();
        GroupElements<Surf> surfs;
        CoordI3Group cG;
        for (UInt i = 0; i < quadBoxes.size(); i++) {
            surfs.add(new QuaI4(cG, ElementId(0), quadBoxes[i]), true);
        }

        Waveport::ExcitationMode excMode = Waveport::TE;
        pair<UInt,UInt> mode(1,0);
        wp_ = new WaveportRectangular(NULL, surfs, excMode, mode);

        pair<OptionsMesher::BoundType,OptionsMesher::BoundType> bound;
        bound.first = OptionsMesher::pml;
        bound.second = OptionsMesher::pml;
        bounds_.resize(3, bound);
    }

    void TearDown() {
        if (wp_ != NULL) {
            delete wp_;
        }
    }

protected:
    Waveport* wp_;
    BoundTerminations bounds_;
};

TEST_F(EMSourceWaveportRectangularTest, Symmetries) {
    // TODO
}

TEST_F(EMSourceWaveportRectangularTest, LocalAndGlobalAxisConversion) {
//    EXPECT_EQ(wp_->getNormal(), CVecR3(0.0,0.0,1.0));
//    vector<CVecR3> global, converted;
//    global.push_back(CVecR3(1.0, 2.0, 3.0));
//    global.push_back(CVecR3(0.0, 3.0, 5.0));
//    converted = wp_->toLocalAxis(global);
//    converted = wp_->toGlobalAxis(converted);
//    for (UInt i = 0; i < converted.size(); i++) {
//        EXPECT_EQ(global[i], converted[i]);
//    }
}

TEST_F(EMSourceWaveportRectangularTest, Weights) {
    // TODO
}

