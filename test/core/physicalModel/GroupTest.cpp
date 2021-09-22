

#include "GroupTest.h"

using namespace std;
using namespace SEMBA;
using namespace PhysicalModel;

TEST_F(PhysicalModelGroupTest, ctor) {
    PMGroup pm;
    EXPECT_NO_THROW(pm.add(
            new Volume::Classic(Id(1), "Classic", 1.0, 1.0, 0.0, 0.0)));
    EXPECT_NO_THROW(pm.add(new Bound::PEC(Id(2))));
}

TEST_F(PhysicalModelGroupTest, bound) {
    PMGroup bounds;
    bounds.add(new Bound::PEC(Id(1)));
    bounds.add(new Bound::PMC(Id(2)));

    EXPECT_NO_THROW(bounds.get(0));
    const SEMBA::PhysicalModel::PhysicalModel& bound = *bounds.get(0);
    EXPECT_TRUE(bound.is<Bound::PEC>());
}
