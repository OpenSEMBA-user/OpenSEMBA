

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

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
