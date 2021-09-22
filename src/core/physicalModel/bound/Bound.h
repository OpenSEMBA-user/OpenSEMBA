

#pragma once

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

class Bound : public virtual PhysicalModel {
public:
    Bound();
    virtual ~Bound();
    static const Bound* strToType(std::string str);
};

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

