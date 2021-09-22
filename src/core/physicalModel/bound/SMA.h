

#pragma once


#include "Bound.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

class SMA : public virtual Bound {
public:
    SMA(const Id id);
    SMA(const SMA& rhs);
    virtual ~SMA();

    SEMBA_CLASS_DEFINE_CLONE(SMA);

    void printInfo() const;
};

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

