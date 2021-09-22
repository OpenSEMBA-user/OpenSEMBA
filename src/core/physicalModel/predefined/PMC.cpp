

#include "PMC.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Predefined {

PMC::PMC(const Id id)
:   Identifiable<Id>(id),
    PhysicalModel("PMC") {

}

PMC::PMC(const Id id, const std::string name)
:   Identifiable<Id>(id),
    PhysicalModel(name) {

}

PMC::PMC(const PMC& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {

}

PMC::~PMC() {

}

void PMC::printInfo() const {
    std::cout << "--- PMC info ---" << std::endl;
    Predefined::printInfo();
    std::cout << "Perfect Magnetic Conductor" << std::endl;
}

} /* namespace Predefined */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
