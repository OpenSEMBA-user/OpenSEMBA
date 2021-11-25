#include "Multiport.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Multiport {

Multiport::Multiport(const Id& id, const std::string& name, const Type& type) :
    Identifiable<Id>(id),
    PhysicalModel(name),
    type_(type)
{

}

Multiport::Type Multiport::getType() const {
    return type_;
}

std::string Multiport::getTypeStr() const {
    switch (type_) {
    case Type::shortCircuit:
        return "Short circuit";
    case Type::openCircuit:
        return "Open circuit";
    case Type::matched:
        return "Matched";
    case Type::sRLC:
        return "Series RLC";
    case Type::sRLpC:
        return "Series RL Parallel C";
    case Type::sRCpL:
        return "Series RC Parallel L";
    case Type::sRpLC:
        return "Series R Parallel C";
    case Type::sLCpR:
        return "Series LC Parallel R";
    case Type::sLpRC:
        return "Series L Parallel RC";
    case Type::sCpLR:
        return "Series C Parallel LR";
    case Type::pRLC:
        return "Parallel RLC";
    case Type::dispersive:
        return "Dispersive";
    default:
        return "Undefined";    
    }
}

} /* namespace Multiport */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
