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
#include "gtest/gtest.h"

#include "source/port/TEMCoaxial.h"

#include "geometry/element/Quadrilateral4.h"

using namespace SEMBA;
using namespace Source;
using namespace std;

class SourcePortTEMCoaxialTest : public ::testing::Test {
    void SetUp() {
        Geometry::BoxI3 plane(Math::CVecI3(0,0,0), Math::CVecI3(30,10,0));
        vector<Geometry::BoxI3> quadBoxes = plane.chop();
        Geometry::ElemId id(0);
        for (size_t i = 0; i < quadBoxes.size(); i++) {
            surfs_.add(new Geometry::QuaI4(cG_, ++id,quadBoxes[i], nullptr, nullptr));
        }

        excMode_ = Port::TEM::voltage;
        innerRadius_ = 1.0;
        outerRadius_ = 3.0;
    }

protected:
    Geometry::CoordI3Group cG_;
    Geometry::Element::Group<Geometry::Surf> surfs_;
    Port::TEM::ExcitationMode excMode_;
    Math::Real innerRadius_, outerRadius_;
};

TEST_F(SourcePortTEMCoaxialTest, basic) {
    Port::TEMCoaxial port(nullptr, surfs_, excMode_,
            Math::CVecR3(0.0), innerRadius_, outerRadius_);

    EXPECT_EQ(Math::CVecR3(0.0,1.0,0.0),
            port.getWeight(Math::CVecR3(0.0,innerRadius_,0.0)).normalize());
    EXPECT_EQ(Math::CVecR3(1.0,0.0,0.0),
            port.getWeight(Math::CVecR3(innerRadius_,0.0,0.0)).normalize());
}
