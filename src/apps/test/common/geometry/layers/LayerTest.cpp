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




