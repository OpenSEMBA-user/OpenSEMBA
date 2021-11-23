

#include <physicalModel/wire/Extremes.h>

namespace SEMBA {
namespace PhysicalModel {
namespace Wire {

Extremes::Extremes(const std::string& name,
                   const Wire& wire,
                   const Multiport::Multiport* extremeL,
                   const Multiport::Multiport* extremeR)
:   Identifiable<Id>(wire),
    PhysicalModel(name),
    Wire(wire) {
    extreme_[0] = extreme_[1] = nullptr;
    if (extremeL != nullptr) {
        extreme_[0] = new Multiport::Multiport(*extremeL);
    }
    if (extremeR != nullptr) {
        extreme_[1] = new Multiport::Multiport(*extremeR);
    }
}

Extremes::Extremes(const Extremes& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs),
    Wire(rhs) {
    extreme_[0] = extreme_[1] = nullptr;
    if (rhs.extreme_[0] != nullptr) {
        extreme_[0] = new Multiport::Multiport(*rhs.extreme_[0]);
    }
    if (rhs.extreme_[1] != nullptr) {
        extreme_[1] = new Multiport::Multiport(*rhs.extreme_[1]);
    }
}

Extremes::~Extremes() {
    if (extreme_[0] != nullptr) {
        delete extreme_[0];
    }
    if (extreme_[1] != nullptr) {
        delete extreme_[1];
    }
}

void Extremes::setExtreme(const std::size_t i,
                          const Multiport::Multiport* extreme) {
    if (extreme_[i] != nullptr) {
        delete extreme_[i];
    }
    extreme_[i] = nullptr;
    if (extreme != nullptr) {
        extreme_[i] = new Multiport::Multiport(*extreme);
    }
}

void Extremes::swapExtremes() {
    std::swap(extreme_[0], extreme_[1]);
}

} /* namespace Wire */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
