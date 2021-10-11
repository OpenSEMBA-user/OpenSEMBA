

#pragma once

#include "Bound.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

class PML : public virtual Bound {
public:
    PML(const Id id);
    PML(const PML&);

    SEMBA_CLASS_DEFINE_CLONE(PML);
};

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

