

#pragma once


#include "Bound.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

class Mur1 : public virtual Bound {
public:
    Mur1(const Id id);
    Mur1(const Mur1&);
    virtual ~Mur1();

    SEMBA_CLASS_DEFINE_CLONE(Mur1);

    void printInfo() const;
};

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

