

#include "ClassicTest.h"

using namespace std;
using namespace SEMBA;
using namespace PhysicalModel;

TEST_F(PhysicalModelVolumeClassicTest, ctor) {
    EXPECT_NO_THROW(Volume::Classic(Id(1), "Classic", 1.0, 1.0, 0.0, 0.0));
}

TEST_F(PhysicalModelVolumeClassicTest, isVacuum) {
    Volume::Classic vacuum(Id(1), "Vacuum", 1.0, 1.0, 0.0, 0.0);
    EXPECT_TRUE(vacuum.isVacuum());
}
