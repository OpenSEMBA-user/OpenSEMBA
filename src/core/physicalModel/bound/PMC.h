

#pragma once


#include "Bound.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

class PMC : public virtual Bound {
public:
    PMC(const Id id);
    PMC(const PMC&);
    virtual ~PMC();

    SEMBA_CLASS_DEFINE_CLONE(PMC);

    void printInfo() const;
};

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

