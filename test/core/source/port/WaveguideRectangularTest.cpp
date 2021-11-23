
#include "gtest/gtest.h"

#include "source/port/WaveguideRectangular.h"

#include "geometry/element/Quadrilateral4.h"
#include "physicalModel/Group.h"
#include "physicalModel/Bound.h"

using namespace SEMBA;
using namespace Source;
using namespace PhysicalModel;
using namespace std;

class SourcePortWaveguideRectangularTest : public ::testing::Test {
    void SetUp() {
        Geometry::BoxI3 plane(Math::CVecI3(0,0,0), Math::CVecI3(30,10,0));
        vector<Geometry::BoxI3> quadBoxes = plane.chop();
        Geometry::ElemId id(0);
        for (size_t i = 0; i < quadBoxes.size(); i++) {
            surfs.add(new Geometry::QuaI4(cG_, ++id,quadBoxes[i], nullptr, nullptr));
        }

        excMode = Port::Waveguide::ExcitationMode::TE;
        mode = pair<size_t,size_t>(1,0);

        boundType.add( std::make_unique <Bound>(MatId(1), Bound::Type::pml));
        boundType.add( std::make_unique <Bound>(MatId(2), Bound::Type::pmc));
        boundType.add( std::make_unique <Bound>(MatId(3), Bound::Type::pec));

        bounds[0][0] = boundType.getId(MatId(1))->castTo<Bound>();
        bounds[1][1] = boundType.getId(MatId(1))->castTo<Bound>();
        bounds[2][0] = boundType.getId(MatId(1))->castTo<Bound>();
        bounds[0][1] = boundType.getId(MatId(1))->castTo<Bound>();
        bounds[1][0] = boundType.getId(MatId(1))->castTo<Bound>();
        bounds[2][1] = boundType.getId(MatId(1))->castTo<Bound>();
    }

protected:
    Geometry::CoordI3Group cG_;
    Geometry::Element::Group<Geometry::Surf> surfs;
    Port::Waveguide::ExcitationMode excMode;
    pair<size_t,size_t> mode;
    Port::Bound3 bounds;
    PMGroup boundType;
};

TEST_F(SourcePortWaveguideRectangularTest, withoutSymmetries) {
    Port::WaveguideRectangular wp(nullptr, surfs, excMode, mode);

    EXPECT_EQ(Math::CVecR3(0.0), wp.getOrigin());
    EXPECT_EQ(30.0, wp.getWidth());
    EXPECT_EQ(10.0, wp.getHeight());

    Math::CVecR3 midPoint = wp.getOrigin();
    midPoint(Math::Constants::x) = wp.getWidth() / 2.0;

    EXPECT_EQ(Math::CVecR3(0.0,1.0,0.0), wp.getWeight(midPoint));

    EXPECT_EQ(Math::CVecR3(0.0), wp.getWeight(wp.getOrigin()));
}