#pragma once

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Multiport {

class Multiport : public virtual PhysicalModel {
public:
    enum Type {
        // PredefinedBase
        shortCircuit = 1,
        openCircuit = 2,
        matched = 3,
        // RLC types.
        sRLC = 11,    // R+L+C.
        sRLpC = 12,   // (R + L)//C.
        sRCpL = 13,   // (R + C)//L.
        sRpLC = 14,   // R + (L//C).
        sLCpR = 15,   // (L + C)//R.
        sLpRC = 16,   // L + (R//C).
        sCpLR = 17,   // C + (L//R).
        pRLC = 18,     // R//L//C.
        // Dispersive
        dispersive = 100,
        undefined = 0
    };
    Multiport();
    virtual ~Multiport();

    SEMBA_CLASS_DEFINE_CLONE(Multiport);

    virtual Type getType() const;

protected:
    Type type_;
    std::string getTypeStr() const;
};

} /* namespace Multiport */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

