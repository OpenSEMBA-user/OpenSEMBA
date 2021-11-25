#include "gtest/gtest.h"

#include "physicalModel/wire/Extremes.h"
#include "physicalModel/multiport/Predefined.h"

namespace SEMBA {
namespace PhysicalModel {

class ExtremesTest : public ::testing::Test {
public:

protected:

};

TEST_F(ExtremesTest, build_from_wire_and_multiport)
{
    Wire::Wire wire(Id(2), "Cable", 1.0, 1.0, 1.0);
    Multiport::Predefined mp1(Id(3), "short", Multiport::Multiport::Type::shortCircuit);
    Multiport::Predefined mp2(Id(4), "open", Multiport::Multiport::Type::openCircuit);
    
    Wire::Extremes extremes("extremes", wire, &mp1, &mp2);

    EXPECT_EQ(Id(2), extremes.getId());
    EXPECT_EQ("extremes", extremes.getName()); 

    EXPECT_EQ(mp1.getType(), extremes.getExtreme(0)->getType());
    EXPECT_EQ(mp2.getType(), extremes.getExtreme(1)->getType());

    Wire::Extremes copied(extremes);
    EXPECT_EQ(Id(2),         copied.getId());
    EXPECT_EQ("extremes",    copied.getName());
    EXPECT_EQ(mp1.getType(), copied.getExtreme(0)->getType());
    EXPECT_EQ(mp2.getType(), copied.getExtreme(1)->getType());

}


}
}