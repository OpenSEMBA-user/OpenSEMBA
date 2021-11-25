

#pragma once

#include "Multiport.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Multiport {

class Dispersive : public virtual Multiport {
public:
    Dispersive(const Id idIn,
               const std::string nameIn,
               const std::string filename);
    Dispersive(const Dispersive&);
    virtual ~Dispersive() = default;

    virtual std::unique_ptr<PhysicalModel> clone() const override {
        return std::make_unique<Dispersive>(*this);
    }

    std::string getFilename() const;

private:
    std::string filename_;
};

} /* namespace Multiport */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

