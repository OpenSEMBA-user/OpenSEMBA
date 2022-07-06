#include "Bound.h"

namespace SEMBA {
namespace PhysicalModel {

Bound::Bound(const Bound& rhs) :
    Identifiable<Id>(rhs),
    PhysicalModel(rhs)
{
    type = rhs.type;
}

Bound::Bound(Id id, Type typeIn) : Identifiable<Id>(id), type(typeIn)
{
    std::string boundName;
    if (type == Type::pec) {
        boundName = "PEC";
    }
    else if (type == Type::pmc) {
        boundName = "PMC";
    }
    else if (type == Type::pml) {
        boundName = "PML";
    }
    else if (type == Type::periodic) {
        boundName = "Periodic";
    }
    else if (type == Type::mur1) {
        boundName = "MUR1";
    }
    else if (type == Type::mur2) {
        boundName = "MUR2";
    }
    else {
        throw std::logic_error("Unrecognized value in Bound ctor.");
    }
    
    setName(boundName + "_Bound");
}

Bound::Type Bound::getType() const {
    return type;
}

} /* namespace PhysicalModel */
} /* namespace SEMBA */
