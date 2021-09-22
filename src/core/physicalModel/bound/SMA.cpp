

#include "SMA.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

SMA::SMA(const Id id)
:   Identifiable<Id>(id), 
    PhysicalModel("SMA") {

}

SMA::SMA(const SMA& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {

}

SMA::~SMA() {

}

void SMA::printInfo() const {
    std::cout << "--- SMA info ---" << std::endl;
    std::cout << "Silver-Muller Absorbing" << std::endl;
}

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
