

#include "Mur2.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

Mur2::Mur2(const Id id)
:   Identifiable<Id>(id),
    PhysicalModel("Mur2") {

}

Mur2::Mur2(const Mur2& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {

}

Mur2::~Mur2() {

}


} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
