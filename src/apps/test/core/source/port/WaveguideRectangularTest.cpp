// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
/*
 * WaveportRectangularTest.cpp
 *
 *  Created on: May 4, 2015
 *      Author: luis
 */

#include "gtest/gtest.h"
//
//#include "../../../../common/sources/PortWaveguideRectangular.h"
//
//class EMSourceWaveportRectangularTest : public ::testing::Test {
//    void SetUp() {
//        BoxI3 plane(CVecI3(0,0,0), CVecI3(30,10,0));
//        vector<BoxI3> quadBoxes = plane.chop();
//        GroupElements<Surf> surfs;
//        CoordI3Group cG;
//        for (UInt i = 0; i < quadBoxes.size(); i++) {
//            surfs.add(new QuaI4(cG, ElementId(0), quadBoxes[i]), true);
//        }
//
//        PortWaveguide::ExcitationMode excMode = PortWaveguide::TE;
//        pair<UInt,UInt> mode(1,0);
//        wp_ = new PortWaveguideRectangular(NULL, surfs, excMode, mode);
//
//        pair<OptionsMesher::BoundType,OptionsMesher::BoundType> bound;
//        bound.first = OptionsMesher::pml;
//        bound.second = OptionsMesher::pml;
//        bounds_.resize(3, bound);
//    }
//
//    void TearDown() {
//        if (wp_ != NULL) {
//            delete wp_;
//        }
//    }
//
//protected:
//    PortWaveguide* wp_;
//    BoundTerminations bounds_;
//};
//
//TEST_F(EMSourceWaveportRectangularTest, Symmetries) {
//    // TODO
//}
//
//TEST_F(EMSourceWaveportRectangularTest, LocalAndGlobalAxisConversion) {
//    EXPECT_EQ(wp_->getNormal(), CVecR3(0.0,0.0,1.0));
//    vector<CVecR3> global, converted;
//    global.push_back(CVecR3(1.0, 2.0, 3.0));
//    global.push_back(CVecR3(0.0, 3.0, 5.0));
//    converted = wp_->toLocalAxis(global);
//    converted = wp_->toGlobalAxis(converted);
//    for (UInt i = 0; i < converted.size(); i++) {
//        EXPECT_EQ(global[i], converted[i]);
//    }
//}
//
//TEST_F(EMSourceWaveportRectangularTest, Weights) {
//    // TODO
//}
//
