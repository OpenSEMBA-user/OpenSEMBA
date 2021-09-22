

#include "Mur2.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

Mur2::Mur2(const Id id)
:   Identifiable<Id>(id),
    PhysicalModel("Mur2") {

}

Mur2::Mur2(const Mur2& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {

}

Mur2::~Mur2() {

}

void Mur2::printInfo() const {
    std::cout << "--- Mur2 info ---" << std::endl;
    std::cout << "Mur2 Boundary Condition" << std::endl;
}

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
