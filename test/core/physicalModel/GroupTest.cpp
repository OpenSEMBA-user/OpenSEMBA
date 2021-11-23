#include "gtest/gtest.h"

#include "physicalModel/Group.h"
#include "physicalModel/Bound.h"
#include "physicalModel/volume/Classic.h"

class PhysicalModelGroupTest : public ::testing::Test {
public:

protected:

};

using namespace std;
using namespace SEMBA;
using namespace PhysicalModel;

TEST_F(PhysicalModelGroupTest, ctor) 
{
    PMGroup pm;
    EXPECT_NO_THROW(
        pm.add(std::make_unique<Volume::Classic>(Id(1), "Classic", 1.0, 1.0, 0.0, 0.0))
    );
    EXPECT_NO_THROW(
        pm.add(std::make_unique<Bound>(Id(2), Bound::Type::pec) )
    );
}

TEST_F(PhysicalModelGroupTest, bound) 
{
    PMGroup bounds;
    bounds.add(std::make_unique<Bound>( Id(1), Bound::Type::pec ));
    bounds.add(std::make_unique<Bound>( Id(2), Bound::Type::pmc ));

    const Bound* bound = bounds.getOf<Bound>()[0];
    EXPECT_TRUE(bound->getType() == Bound::Type::pec);
}
