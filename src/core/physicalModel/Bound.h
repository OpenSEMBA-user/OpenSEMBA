

#pragma once

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {

class Bound : public virtual PhysicalModel {
public:
    enum class Type {
        mur1,
        mur2,
        pec,
        pmc,
        periodic,
        pml,
        sma
    };
    
    Bound(Id id, Type type);
    virtual ~Bound() = default;

    SEMBA_CLASS_DEFINE_CLONE(Bound);
    
    Type getType() const;
private:
    Type type;
};

} /* namespace PhysicalModel */
} /* namespace SEMBA */

