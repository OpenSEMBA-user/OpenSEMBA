

#include "PEC.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Predefined {

PEC::PEC(const Id id)
:   Identifiable<Id>(id),
    PhysicalModel("PEC") {

}


PEC::PEC(const Id id, const std::string name)
:   Identifiable<Id>(id),
    PhysicalModel(name) {

}

PEC::PEC(const PEC& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {

}




} /* namespace Predefined */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
