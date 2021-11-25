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

    virtual std::unique_ptr<PhysicalModel> clone() const override {
        return std::make_unique<Predefined>(*this);
    }

};

} /* namespace Multiport */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

