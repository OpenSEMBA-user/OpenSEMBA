

#pragma once


#include "Bound.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

class SMA : public virtual Bound {
public:
    SMA(const Id id);
    SMA(const SMA& rhs);

    SEMBA_CLASS_DEFINE_CLONE(SMA);

};

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

