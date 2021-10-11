

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


} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
