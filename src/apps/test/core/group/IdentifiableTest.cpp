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
 * GroupIdTest.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: luis
 */

#include "gtest/gtest.h"
#include "class/Identifiable.h"
#include "geometry/layer/Group.h"

using namespace std;

using namespace SEMBA;
using namespace Class;
using namespace Geometry;
using namespace Layer;


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

    void areEqual(const vector<Layer*>& vec, const Group<>& layers) {
        for (size_t i = 0; i < vec.size(); i++) {
            EXPECT_EQ(*vec[i], *layers.getId(vec[i]->getId()));
        }
    }
};

TEST_F(BaseGroupGroupIdTest, ctor) {
    vector<Layer*> vecLayers = newLayersVector();
    Group<> layers(vecLayers);
    areEqual(vecLayers, layers);
}
