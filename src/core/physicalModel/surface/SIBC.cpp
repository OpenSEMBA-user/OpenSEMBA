

#include <physicalModel/surface/SIBC.h>

namespace SEMBA {
namespace PhysicalModel {
namespace Surface {

SIBC::SIBC(const Id id,
        const std::string& name,
        const Math::MatC22& Zinfinite,
        const Math::MatC22& ZLinear,
        const std::vector<PoleResidue>& poleImpedance)
:   Identifiable<Id>(id),
    PhysicalModel(name),
    ZInfinity_(Zinfinite),
    ZLinear_(ZLinear),
    poleZ_(poleImpedance) {
}

SIBC::~SIBC() {

}

std::size_t SIBC::getNumberOfPoles() const {
    return poleZ_.size();
}

} /* namespace Surface */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
