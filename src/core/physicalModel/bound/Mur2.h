

#pragma once


#include "Bound.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

class Mur2 : public virtual Bound {
public:
    Mur2(const Id id);
    Mur2(const Mur2&);
    virtual ~Mur2();

    SEMBA_CLASS_DEFINE_CLONE(Mur2);

    void printInfo() const;
};

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

