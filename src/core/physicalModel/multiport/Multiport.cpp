

#include "Multiport.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Multiport {

Multiport::Multiport() {
    type_ = undefined;
}

Multiport::~Multiport() {

}

Multiport::Type Multiport::getType() const {
    return type_;
}

std::string Multiport::getTypeStr() const {
    std::string res;
    switch (type_) {
    case shortCircuit:
        res = "Short circuit";
        break;
    case openCircuit:
        res = "Open circuit";
        break;
    case matched:
        res = "Matched";
        break;
    case sRLC:
        res = "Series RLC";
        break;
    case sRLpC:
        res = "Series RL Parallel C";
        break;
    case sRCpL:
        res = "Series RC Parallel L";
        break;
    case sRpLC:
        res = "Series R Parallel C";
        break;
    case sLCpR:
        res = "Series LC Parallel R";
        break;
    case sLpRC:
        res = "Series L Parallel RC";
        break;
    case sCpLR:
        res = "Series C Parallel LR";
        break;
    case pRLC:
        res = "Parallel RLC";
        break;
    case dispersive:
        res = "Dispersive";
        break;
    default:
        res = "Undefined";
        break;
    }
    return res;
}

} /* namespace Multiport */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
