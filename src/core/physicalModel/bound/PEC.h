

#pragma once

#include "Bound.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

class PEC : public virtual Bound {
public:
    PEC(const Id id);
    PEC(const PEC&);
    virtual ~PEC();

    SEMBA_CLASS_DEFINE_CLONE(PEC);

    void printInfo() const;
};

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

