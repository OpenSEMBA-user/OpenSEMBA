
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
            surfs.add(new Geometry::QuaI4(cG_, ++id,quadBoxes[i], nullptr, nullptr));
        }

        excMode = Port::TEM::voltage;
        innerRadius_ = 1.0;
        outerRadius_ = 3.0;
    }

protected:
    Geometry::CoordI3Group cG_;
    Geometry::Element::Group<Geometry::Surf> surfs;
    Port::TEM::ExcitationMode excMode;
    Math::Real innerRadius_, outerRadius_;
};

TEST_F(SourcePortTEMCoaxialTest, basic) {
    Port::TEMCoaxial port(nullptr, surfs, excMode,
            Math::CVecR3(0.0), innerRadius_, outerRadius_);

    EXPECT_EQ(Math::CVecR3(0.0,1.0,0.0),
            port.getWeight(Math::CVecR3(0.0,innerRadius_,0.0)).normalize());
    EXPECT_EQ(Math::CVecR3(1.0,0.0,0.0),
            port.getWeight(Math::CVecR3(innerRadius_,0.0,0.0)).normalize());
}
