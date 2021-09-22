

#pragma once


#include "Bound.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

class Periodic : public virtual Bound {
public:
    Periodic(const Id id);
    Periodic(const Periodic&);
    virtual ~Periodic();

    SEMBA_CLASS_DEFINE_CLONE(Periodic);

    void printInfo() const;
};

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

