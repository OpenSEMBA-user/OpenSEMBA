

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


} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
