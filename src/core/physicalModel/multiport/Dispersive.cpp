

#include "Dispersive.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Multiport {

Dispersive::Dispersive(const Id id,
                       const std::string name,
                       const std::string filename)
:   Identifiable<Id>(id),
    PhysicalModel(name) {
    filename_ = filename;
    type_ = dispersive;
}

Dispersive::Dispersive(const Dispersive& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {
    filename_ = rhs.filename_;
    type_ = rhs.type_;
}

Dispersive::~Dispersive() {

}

std::string Dispersive::getFilename() const {
    return filename_;
}


} /* namespace Multiport */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
