

#include "Predefined.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Predefined {

Predefined::Predefined() {
}

Predefined::~Predefined() {
}

void Predefined::printInfo() const {
    std::cout << "--- Predefined info ---" << std::endl;
    PhysicalModel::printInfo();
}

} /* namespace Predefined */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
