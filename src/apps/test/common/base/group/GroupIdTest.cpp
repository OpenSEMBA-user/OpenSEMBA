/*
 * GroupIdTest.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: luis
 */

#include "gtest/gtest.h"
#include "base/group/GroupId.h"
#include "geometry/layers/GroupLayers.h"

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

    void areEqual(const vector<Layer*>& vec, const GroupLayers<>& layers) {
        for (UInt i = 0; i < vec.size(); i++) {
            EXPECT_EQ(*vec[i], *layers.getId(vec[i]->getId()));
        }
    }
};

TEST_F(BaseGroupGroupIdTest, ctor) {
    vector<Layer*> vecLayers = newLayersVector();
    GroupLayers<> layers(vecLayers);
    areEqual(vecLayers, layers);
}
