

#include "PEC.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

// =============== PEC ======================================================
PEC::PEC(const Id id)
:   Identifiable<Id>(id),
    PhysicalModel("PEC") {

}

PEC::PEC(const PEC& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {

}

PEC::~PEC() {

}

void PEC::printInfo() const {
    std::cout << "--- PEC info ---" << std::endl;
    std::cout << "Perfect Electric Conductor" << std::endl;
}

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
