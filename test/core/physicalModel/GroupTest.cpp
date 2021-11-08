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

TEST_F(PhysicalModelGroupTest, ctor) {
    PMGroup pm;
    EXPECT_NO_THROW(pm.add(
            new Volume::Classic(Id(1), "Classic", 1.0, 1.0, 0.0, 0.0))
    );
    EXPECT_NO_THROW(pm.add(new Bound{ Id(2), Bound::Type::pec } ));
}

TEST_F(PhysicalModelGroupTest, bound) {
    PMGroup bounds;
    bounds.add(new Bound{ Id(1), Bound::Type::pec });
    bounds.add(new Bound{ Id(2), Bound::Type::pmc });

    EXPECT_NO_THROW(bounds.get(0));
    const Bound* bound = bounds.getOf<Bound>().get(0);
    EXPECT_TRUE(bound->getType() == Bound::Type::pec);
}
