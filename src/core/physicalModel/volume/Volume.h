

#pragma once

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Volume {

class Volume : public virtual PhysicalModel {
public:
    Volume();
    virtual ~Volume();

    virtual void printInfo() const;
};

} /* namespace Volume */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

