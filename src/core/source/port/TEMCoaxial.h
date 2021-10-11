

#pragma once

#include "TEM.h"

namespace SEMBA {
namespace Source {
namespace Port {

class TEMCoaxial : public TEM {
public:
    TEMCoaxial(
            Magnitude::Magnitude* magnitude,
            const Geometry::Element::Group<const Geometry::Surf>& elem,
            const ExcitationMode excMode,
            const Math::CVecR3& origin,
            const Math::Real innerRadius,
            const Math::Real outerRadius);
    TEMCoaxial(const TEMCoaxial& rhs);
    virtual ~TEMCoaxial();

    SEMBA_CLASS_DEFINE_CLONE(TEMCoaxial);

    void set(const Geometry::Element::Group<const Geometry::Elem>&);

    const std::string& getName() const;

    Math::CVecR3 getOrigin() const;
    Math::CVecR3 getWeight(const Math::CVecR3& pos) const;

private:
    Math::CVecR3 origin_;
    Math::Real innerRadius_, outerRadius_;
};

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */

