

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


} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
