#pragma once

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Surface {

class Surface : public virtual PhysicalModel {
public:
    Surface() = default;
    virtual ~Surface() = default;
};

} /* namespace Surface */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

