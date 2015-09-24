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
 * GeometryLayersLayerTest.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: luis
 */

#include "gtest/gtest.h"
#include "geometry/layers/Layer.h"

class GeometryLayersLayerTest : public ::testing::Test {
public:
    GeometryLayersLayerTest() {};
    virtual ~GeometryLayersLayerTest() {};

protected:

};

TEST_F(GeometryLayersLayerTest, comparison) {
    Layer lay1P(LayerId(1), "Patata");
    Layer lay1T(LayerId(1), "Tomate");
    Layer lay2T(LayerId(2), "Tomate");

    EXPECT_EQ(lay1P, lay1P);
    EXPECT_NE(lay1P, lay1T);
    EXPECT_NE(lay1P, lay2T);
}




