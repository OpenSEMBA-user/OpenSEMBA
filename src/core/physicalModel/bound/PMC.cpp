

#include "PMC.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

PMC::PMC(const Id id)
:   Identifiable<Id>(id),
    PhysicalModel("PMC") {

}

PMC::PMC(const PMC& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {

}

PMC::~PMC() {

}

void PMC::printInfo() const {
    std::cout << "--- PMC info ---" << std::endl;
    std::cout << "Perfect Magnetic Conductor" << std::endl;
}

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
