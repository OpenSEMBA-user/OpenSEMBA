#pragma once

#include "Bound.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

class Periodic : public virtual Bound {
public:
    Periodic(const Id id);
    Periodic(const Periodic&);

    SEMBA_CLASS_DEFINE_CLONE(Periodic);

};

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

