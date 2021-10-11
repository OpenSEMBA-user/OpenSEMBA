

#include <physicalModel/gap/Gap.h>

namespace SEMBA {
namespace PhysicalModel {
namespace Gap {

Gap::Gap(const Id id,
           const std::string name,
           const Math::Real width)
:   Identifiable<Id>(id),
    PhysicalModel(name) {
    width_ = width;
}

Gap::Gap(const Gap& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {
    width_ = rhs.width_;
}


Math::Real Gap::getWidth() const {
    return width_;
}



} /* namespace Gap */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
