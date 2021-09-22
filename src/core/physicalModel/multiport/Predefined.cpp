

#include <physicalModel/multiport/Predefined.h>

namespace SEMBA {
namespace PhysicalModel {
namespace Multiport {

Predefined::Predefined(const Id id,
        const std::string name,
        const Multiport::Type type)
:   Identifiable<Id>(id),
    PhysicalModel(name) {
    type_ = type;
}

Predefined::Predefined(const Predefined& rhs)
: Identifiable<Id>(rhs),
  PhysicalModel(rhs) {
    type_ = rhs.type_;
}

Predefined::~Predefined() {

}

void Predefined::printInfo() const {
    std::cout<< " --- Multiport Info --- " << std::endl;
    Multiport::printInfo();
    std::cout<< " Type: " << getTypeStr() << std::endl;
}

} /* namespace Multiport */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
