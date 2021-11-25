#pragma once

#include "geometry/Box.h"
#include "math/axis/Local.h"

#include "Volume.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Volume {

class PML : public virtual Volume {
public:
    PML(const Id id, const std::string& name, const Math::Axis::Local orientation);
    PML(const PML& rhs);
    virtual ~PML() = default;
  
    virtual std::unique_ptr<PhysicalModel> clone() const override {
        return std::make_unique<PML>(*this);
    }

    const Math::Axis::Local getOrientation() const;
    const Math::CVecR3 getGlobalZAxis() const;

private:
    const Math::Axis::Local orientation_;
};

} /* namespace Volume */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

