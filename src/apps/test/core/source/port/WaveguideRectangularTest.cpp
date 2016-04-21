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

#include "source/port/WaveguideRectangular.h"

#include "geometry/element/Quadrilateral4.h"
#include "physicalModel/Group.h"
#include "physicalModel/bound/PML.h"
#include "physicalModel/bound/PMC.h"
#include "physicalModel/bound/PEC.h"

using namespace SEMBA;
using namespace Source;
using namespace std;

class SourcePortWaveguideRectangularTest : public ::testing::Test {
    void SetUp() {
        Geometry::BoxI3 plane(Math::CVecI3(0,0,0), Math::CVecI3(30,10,0));
        vector<Geometry::BoxI3> quadBoxes = plane.chop();
        Geometry::ElemId id(0);
        for (size_t i = 0; i < quadBoxes.size(); i++) {
            surfs_.add(new Geometry::QuaI4(cG_, ++id,quadBoxes[i], NULL, NULL));
        }

        excMode_ = Port::Waveguide::TE;
        mode_ = pair<size_t,size_t>(1,0);

        boundType_.add(new PhysicalModel::Bound::PML(MatId(1)));
        boundType_.add(new PhysicalModel::Bound::PMC(MatId(2)));
        boundType_.add(new PhysicalModel::Bound::PEC(MatId(3)));

        bounds_[0][0] = boundType_.getId(MatId(1))
                        ->castTo<PhysicalModel::Bound::Bound>();
        bounds_[1][1] = boundType_.getId(MatId(1))
                        ->castTo<PhysicalModel::Bound::Bound>();
        bounds_[2][0] = boundType_.getId(MatId(1))
                        ->castTo<PhysicalModel::Bound::Bound>();
        bounds_[0][1] = boundType_.getId(MatId(1))
                        ->castTo<PhysicalModel::Bound::Bound>();
        bounds_[1][0] = boundType_.getId(MatId(1))
                        ->castTo<PhysicalModel::Bound::Bound>();
        bounds_[2][1] = boundType_.getId(MatId(1))
                        ->castTo<PhysicalModel::Bound::Bound>();
    }

protected:
    Geometry::CoordI3Group cG_;
    Geometry::Element::Group<Geometry::Surf> surfs_;
    Port::Waveguide::ExcitationMode excMode_;
    pair<size_t,size_t> mode_;
    Port::Bound3 bounds_;
    PMGroup boundType_;
};

TEST_F(SourcePortWaveguideRectangularTest, withoutSymmetries) {
    Port::WaveguideRectangular wp(NULL, surfs_, excMode_, mode_, bounds_);

    EXPECT_EQ(Math::CVecR3(0.0), wp.getOrigin());
    EXPECT_EQ(30.0, wp.getWidth());
    EXPECT_EQ(10.0, wp.getHeight());

    Math::CVecR3 midPoint = wp.getOrigin();
    midPoint(Math::Constants::x) = wp.getWidth() / 2.0;

    EXPECT_EQ(Math::CVecR3(0.0,1.0,0.0), wp.getWeight(midPoint));

    EXPECT_EQ(Math::CVecR3(0.0),         wp.getWeight(wp.getOrigin()));
}

TEST_F(SourcePortWaveguideRectangularTest, withSymmetry) {
    bounds_[0][0] = boundType_.getId(MatId(2))
                    ->castTo<PhysicalModel::Bound::Bound>();
    bounds_[1][0] = boundType_.getId(MatId(3))
                    ->castTo<PhysicalModel::Bound::Bound>();
    Port::WaveguideRectangular wp(NULL, surfs_, excMode_, mode_, bounds_);

    EXPECT_EQ(Math::CVecR3(-wp.getWidth()/2.0, -wp.getHeight()/2.0, 0.0),
              wp.getOrigin());
    EXPECT_EQ(60.0, wp.getWidth());
    EXPECT_EQ(20.0, wp.getHeight());

    Math::CVecR3 midPoint = wp.getOrigin();
    midPoint(Math::Constants::x) += wp.getWidth() / 2.0;
    midPoint(Math::Constants::y) += wp.getHeight() / 2.0;
    EXPECT_EQ(Math::CVecR3(0.0), midPoint);

    EXPECT_EQ(Math::CVecR3(0.0,1.0,0.0), wp.getWeight(midPoint));

    EXPECT_EQ(Math::CVecR3(0.0),         wp.getWeight(wp.getOrigin()));
}

TEST_F(SourcePortWaveguideRectangularTest, notValidSymmetry) {
    bounds_[0][0] = boundType_.getId(MatId(3))
                    ->castTo<PhysicalModel::Bound::Bound>();
    bounds_[1][0] = boundType_.getId(MatId(3))
                    ->castTo<PhysicalModel::Bound::Bound>();
    EXPECT_ANY_THROW(
            Port::WaveguideRectangular(NULL, surfs_, excMode_, mode_, bounds_));
}
