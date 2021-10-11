

#include "Mur1.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

Mur1::Mur1(const Id id)
:   Identifiable<Id>(id),
    PhysicalModel("Mur1") {

}

Mur1::Mur1(const Mur1& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {

}

Mur1::~Mur1() {

}

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
