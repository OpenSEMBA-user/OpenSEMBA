

#include "Waveguide.h"

namespace SEMBA {
namespace Source {
namespace Port {

Waveguide::Waveguide(SEMBA::Source::Magnitude::Magnitude* magnitude,
        const Geometry::Element::Group<const Geometry::Surf>& elem,
        const ExcitationMode excMode,
        const std::pair<size_t,size_t> mode)
:   Port(magnitude, elem) {

    excitationMode_ = excMode;
    mode_ = mode;
    // Performs checks
    if (!elem.getBound().isSurface()) {
        throw std::logic_error("Waveport elements must be contained "
                               "in a coplanar Geometry::Surface");
    }

    Math::CVecR3 diagonal = elem.getBound().getMax() -
                            elem.getBound().getMin();
    if (!diagonal.isContainedInPlane(Math::Constants::xy)) {
        throw std::logic_error("Waveport must be contained in plane xy.");
    }

    if (elem.size() == 0) {
        throw std::logic_error("Waveport must contain some elements.");
    }
}

Waveguide::Waveguide(const Waveguide& rhs) :
                Port(rhs) {
    excitationMode_ = rhs.excitationMode_;
    mode_ = rhs.mode_;
}

Waveguide::~Waveguide() {

}

Waveguide::ExcitationMode Waveguide::getExcitationMode() const {
    return excitationMode_;
}

std::pair<size_t,size_t> Waveguide::getMode() const {
    return mode_;
}

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */
