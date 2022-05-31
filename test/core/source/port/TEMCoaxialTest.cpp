#include "gtest/gtest.h"

#include "source/port/TEMCoaxial.h"
#include "geometry/element/Quadrilateral4.h"
#include "math/function/Gaussian.h"

using namespace SEMBA;
using namespace Source;
using namespace std;

class SourcePortTEMCoaxialTest : public ::testing::Test {
    void SetUp() {
        Geometry::BoxI3 plane(Math::CVecI3(0,0,0), Math::CVecI3(30,10,0));
        vector<Geometry::BoxI3> quadBoxes = plane.chop();
        Geometry::ElemId id(0);
        for (size_t i = 0; i < quadBoxes.size(); i++) {
            surfs.add(std::make_unique<Geometry::QuaI4>(cG_, ++id,quadBoxes[i], nullptr, nullptr));
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

    static const std::unique_ptr<Magnitude::Magnitude> buildMagnitude() {
        return std::make_unique<Magnitude::Magnitude>(Magnitude::Magnitude(
            new Math::Function::Gaussian(
                Math::Function::Gaussian::buildFromMaximumFrequency(
                    1e9,
                    1.0
                )
            )
        ));
    }
};

TEST_F(SourcePortTEMCoaxialTest, basic) {
    Port::TEMCoaxial port(
        buildMagnitude(),
        surfs.getOf<Geometry::Elem>(), 
        excMode,
        Math::CVecR3(0.0), 
        innerRadius_, 
        outerRadius_
    );

    EXPECT_EQ(Math::CVecR3(0.0,1.0,0.0),
            port.getWeight(Math::CVecR3(0.0,innerRadius_,0.0)).normalize());
    EXPECT_EQ(Math::CVecR3(1.0,0.0,0.0),
            port.getWeight(Math::CVecR3(innerRadius_,0.0,0.0)).normalize());
}
