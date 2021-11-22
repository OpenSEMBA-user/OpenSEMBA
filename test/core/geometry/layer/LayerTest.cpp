
#include "gtest/gtest.h"
#include "geometry/layer/Layer.h"

using namespace SEMBA;
using namespace Geometry;

class GeometryLayersLayerTest : public ::testing::Test {
};

TEST_F(GeometryLayersLayerTest, comparison) {
    Layer::Layer lay1P(LayerId(1), "Patata");
    Layer::Layer lay1T(LayerId(1), "Tomate");
    Layer::Layer lay2T(LayerId(2), "Tomate");

    EXPECT_EQ(lay1P, lay1P);
    EXPECT_NE(lay1P, lay1T);
    EXPECT_NE(lay1P, lay2T);
}




