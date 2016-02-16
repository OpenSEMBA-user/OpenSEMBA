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

#include "IdentifiableTest.h"

using namespace std;

using namespace SEMBA;
using namespace Class;
using namespace Geometry;

vector<Layer::Layer*> IdentifiableTest::newLayersVector() const {
    std::vector<Layer::Layer*> res;
    res.push_back(new Layer::Layer(LayerId(1), "Patata"));
    res.push_back(new Layer::Layer(LayerId(6), "Cebolla"));
    res.push_back(new Layer::Layer(LayerId(5), "Huevos"));
    return res;
}

void IdentifiableTest::areEqual(
        const std::vector<Layer::Layer*>& vec,
        const Layer::Group<>& layers) {
    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_EQ(*vec[i], *layers.getId(vec[i]->getId()));
    }
}

TEST_F(IdentifiableTest, ctor) {
    vector<Layer::Layer*> vecLayers = IdentifiableTest::newLayersVector();
    Layer::Group<> layers(vecLayers);
    areEqual(vecLayers, layers);
}
