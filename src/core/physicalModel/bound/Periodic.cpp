

#include "Periodic.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

Periodic::Periodic(const Id id)
:   Identifiable<Id>(id),
    PhysicalModel("Periodic") {

}

Periodic::Periodic(const Periodic& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {

}

Periodic::~Periodic() {

}

void Periodic::printInfo() const {
    std::cout << "--- Periodic info ---" << std::endl;
    std::cout << "Periodic Boundary Condition" << std::endl;
}

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
