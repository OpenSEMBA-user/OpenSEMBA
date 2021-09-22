

#pragma once


#include "Predefined.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Predefined {

class SMA : public virtual Predefined {
public:
    SMA(const Id id);
    SMA(const Id id, const std::string name);
    SMA(const SMA& rhs);
    virtual ~SMA();

    SEMBA_CLASS_DEFINE_CLONE(SMA);

    void printInfo() const;
};

} /* namespace Predefined */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

