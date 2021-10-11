

#pragma once


#include "Bound.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

class PMC : public virtual Bound {
public:
    PMC(const Id id);
    PMC(const PMC&);

    SEMBA_CLASS_DEFINE_CLONE(PMC);

};

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

