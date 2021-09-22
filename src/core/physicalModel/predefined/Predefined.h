

#pragma once

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Predefined {

class Predefined : public virtual PhysicalModel {
public:
    Predefined();
    virtual ~Predefined();

    virtual void printInfo() const;
};

} /* namespace Predefiend */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

