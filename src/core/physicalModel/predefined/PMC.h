

#pragma once


#include "Predefined.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Predefined {

class PMC : public virtual Predefined {
public:
    PMC(const Id id);
    PMC(const Id id, const std::string name);
    PMC(const PMC&);

    SEMBA_CLASS_DEFINE_CLONE(PMC);

};

} /* namespace Predefined */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

