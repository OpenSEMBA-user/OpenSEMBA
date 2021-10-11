

#pragma once

#include "Predefined.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Predefined {

class PEC : public virtual Predefined {
public:
    PEC(const Id id);
    PEC(const Id id, const std::string name);
    PEC(const PEC&);
    
    SEMBA_CLASS_DEFINE_CLONE(PEC);

};

} /* namespace Predefined */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

