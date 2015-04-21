/*
 * GroupIdTest.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: luis
 */

#include "gtest/gtest.h"
#include "base/class/IdBase.h"
#include "base/group/GroupId.h"
#include "geometry/layers/LayerGroup.h"

class BaseGroupGroupIdTest : public ::testing::Test {
public:
    BaseGroupGroupIdTest() {};
    virtual ~BaseGroupGroupIdTest() {};

protected:
    vector<Layer*> newLayersVector() const {
        vector<Layer*> res;
        res.push_back(new Layer(LayerId(1), "Patata"));
        res.push_back(new Layer(LayerId(6), "Cebolla"));
        res.push_back(new Layer(LayerId(5), "Huevos"));
        return res;
    }

    void areEqual(const vector<Layer*>& vec, const LayerGroup<>& layers) {
        for (UInt i = 0; i < vec.size(); i++) {
            EXPECT_EQ(*vec[i], *layers.get(vec[i]->getId()));
        }
    }
};

TEST_F(BaseGroupGroupIdTest, ctor) {
    vector<Layer*> vecLayers = newLayersVector();
    LayerGroup<> layers(vecLayers);
    areEqual(vecLayers, layers);
}
