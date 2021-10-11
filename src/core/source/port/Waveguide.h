

#pragma once

#include "Port.h"


namespace SEMBA {
namespace Source {
namespace Port {

class Waveguide : public Port {
public:
    typedef enum {
        TE,
        TM
    } ExcitationMode;

    Waveguide(Magnitude::Magnitude* magnitude,
              const Geometry::Element::Group<const Geometry::Surf>& elem,
              const ExcitationMode excMode,
              const std::pair<size_t,size_t> mode);
    Waveguide(const Waveguide& rhs);
    virtual ~Waveguide();

    ExcitationMode getExcitationMode() const;
    std::pair<size_t, size_t> getMode() const;

private:
    ExcitationMode excitationMode_;
    std::pair<size_t,size_t> mode_;
};

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */

