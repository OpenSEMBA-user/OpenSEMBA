#include "physicalModel/wire/Extremes.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Wire {

Extremes::Extremes(const std::string& name,
                   const Wire& wire,
                   const Multiport::Multiport* extremeL,
                   const Multiport::Multiport* extremeR)
:   Wire(wire)
{
    setName(name);
    setId(wire.getId());
    if (extremeL != nullptr) {
        extreme_[0].reset(extremeL->clone().release()->castTo<Multiport::Multiport>());
    }
    if (extremeR != nullptr) {
        extreme_[1].reset(extremeR->clone().release()->castTo<Multiport::Multiport>());
    }
}

Extremes::Extremes(const Extremes& rhs) : 
    Identifiable<Id>(rhs),
    PhysicalModel(rhs),
    Wire(rhs)
{
    if (rhs.extreme_[0] != nullptr) {
        extreme_[0].reset(rhs.extreme_[0]->clone().release()->castTo<Multiport::Multiport>());
    }
    if (rhs.extreme_[1] != nullptr) {
        extreme_[1].reset(rhs.extreme_[1]->clone().release()->castTo<Multiport::Multiport>());
    }
}

void Extremes::setExtreme(const std::size_t i,
                          const Multiport::Multiport* extreme) {
    if (extreme == nullptr) {
        extreme_[i].reset();
    }
    else {
        extreme_[i].reset(extreme->clone().release()->castTo<Multiport::Multiport>());
    }
}

void Extremes::swapExtremes() {
    std::swap(extreme_[0], extreme_[1]);
}

} /* namespace Wire */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
