

#include "SMA.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Predefined {

SMA::SMA(const Id id)
:   Identifiable<Id>(id), 
    PhysicalModel("SMA") {

}

SMA::SMA(const Id id, const std::string name)
:   Identifiable<Id>(id),
    PhysicalModel(name) {

}

SMA::SMA(const SMA& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {

}

SMA::~SMA() {

}

void SMA::printInfo() const {
    std::cout << "--- SMA info ---" << std::endl;
    Predefined::printInfo();
    std::cout << "Silver-Muller Absorbing" << std::endl;
}

} /* namespace Predefined */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
