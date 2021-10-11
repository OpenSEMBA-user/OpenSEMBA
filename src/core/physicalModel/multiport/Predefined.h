

#pragma once

#include "Multiport.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Multiport {

class Predefined : public virtual Multiport {
public:
    Predefined(const Id idIn,
                        const std::string nameIn,
                        const Multiport::Type);
    Predefined(const Predefined&);

    SEMBA_CLASS_DEFINE_CLONE(Predefined);

};

} /* namespace Multiport */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

