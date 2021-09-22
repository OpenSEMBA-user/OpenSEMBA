

#include "PML.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

PML::PML(const Id id)
:   Identifiable<Id>(id),
    PhysicalModel("PML") {

}

PML::PML(const PML& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {

}

PML::~PML() {

}

void PML::printInfo() const {
    std::cout << "--- PML info ---" << std::endl;
    std::cout << "Perfectly Matched Layer" << std::endl;
}

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
