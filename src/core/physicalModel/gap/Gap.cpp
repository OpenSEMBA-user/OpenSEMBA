

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

Gap::~Gap() {

}

Math::Real Gap::getWidth() const {
    return width_;
}

void Gap::printInfo() const {
    std::cout<< " --- Gap info ---" << std::endl;
    PhysicalModel::printInfo();
    std::cout << " Width: " << width_ << std::endl;
}

} /* namespace Gap */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
