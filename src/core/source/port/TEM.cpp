

#include "TEM.h"

namespace SEMBA {
namespace Source {
namespace Port {

TEM::TEM(
        const std::unique_ptr<Magnitude::Magnitude>& magnitude,
        const Target& elem,
        const ExcitationMode excitationMode) : Port(magnitude, elem) {
    excitationMode_ = excitationMode;
}

TEM::TEM(const TEM& rhs) : Port(rhs) {
    excitationMode_ = rhs.excitationMode_;
}

TEM::ExcitationMode TEM::getExcitationMode() const {
    return excitationMode_;
}

std::string TEM::toStr(const ExcitationMode& excitationMode) {
    switch (excitationMode) {
    case ExcitationMode::voltage:
        return std::string("voltage");
    case ExcitationMode::current:
    default:
        return std::string("current");
    }
}

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */
