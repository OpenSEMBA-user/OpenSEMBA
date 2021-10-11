

#pragma once

#include "Port.h"

namespace SEMBA {
namespace Source {
namespace Port {

class TEM : public Port {
public:
    typedef enum {
        voltage,
        current
    } ExcitationMode;

    TEM(Magnitude::Magnitude* magnitude,
        const Geometry::Element::Group<const Geometry::Surf>& elem,
        const ExcitationMode excitationMode);
    TEM(const TEM& rhs);
    virtual ~TEM();

    ExcitationMode getExcitationMode() const;

    static std::string toStr(const ExcitationMode& excitationMode);
private:
    ExcitationMode excitationMode_;
};

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */

