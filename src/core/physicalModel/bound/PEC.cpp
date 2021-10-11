

#include "PEC.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

// =============== PEC ======================================================
PEC::PEC(const Id id)
:   Identifiable<Id>(id),
    PhysicalModel("PEC") {

}

PEC::PEC(const PEC& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {

}

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
