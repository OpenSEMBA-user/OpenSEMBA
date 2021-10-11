

#include <physicalModel/volume/PML.h>

namespace SEMBA {
namespace PhysicalModel {
namespace Volume {

PML::PML(const Id id,
         const std::string& name,
         const Math::Axis::Local orientation)
:   Identifiable<Id>(id),
    PhysicalModel(name),
    orientation_(orientation) {
}

PML::PML(const PML& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs),
    orientation_(rhs.orientation_) {
}

const Math::Axis::Local PML::getOrientation() const {
    return orientation_;
}

const Math::CVecR3 PML::getGlobalZAxis() const {
    Math::CVecR3 localZ(0.0,0.0,1.0);
    Math::CVecR3 res = getOrientation().convertToGlobal(localZ)
            - getOrientation().getOrigin();
    return res;
}

} /* namespace Volume */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

