

#include "Volume.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Volume {

Volume::Volume() {

}

Volume::~Volume() {

}

void Volume::printInfo() const {
    std::cout << "--- Volume info ---" << std::endl;
    PhysicalModel::printInfo();
}

} /* namespace Volume */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
