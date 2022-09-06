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
    setName(getTypeName() + "_Bound");
}

std::string Bound::getTypeName() const {
    if (type == Type::pec) {
        return "PEC";
    }
    else if (type == Type::pmc) {
        return "PMC";
    }
    else if (type == Type::pml) {
        return "PML";
    }
    else if (type == Type::periodic) {
        return "Periodic";
    }
    else if (type == Type::mur1) {
        return "MUR1";
    }
    else if (type == Type::mur2) {
        return "MUR2";
    }// TODO: What happens with SMA?
    else {
        throw std::logic_error("Unrecognized value for Bound type name");
    }
}


Bound::Type Bound::getType() const {
    return type;
}

} /* namespace PhysicalModel */
} /* namespace SEMBA */
