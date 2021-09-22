
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
            surfs_.add(new Geometry::QuaI4(cG_, ++id,quadBoxes[i], nullptr, nullptr));
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
    Port::WaveguideRectangular wp(nullptr, surfs_, excMode_, mode_);

    EXPECT_EQ(Math::CVecR3(0.0), wp.getOrigin());
    EXPECT_EQ(30.0, wp.getWidth());
    EXPECT_EQ(10.0, wp.getHeight());

    Math::CVecR3 midPoint = wp.getOrigin();
    midPoint(Math::Constants::x) = wp.getWidth() / 2.0;

    EXPECT_EQ(Math::CVecR3(0.0,1.0,0.0), wp.getWeight(midPoint));

    EXPECT_EQ(Math::CVecR3(0.0),         wp.getWeight(wp.getOrigin()));
}