#pragma once

#include "TEM.h"

namespace SEMBA {
namespace Source {
namespace Port {

class TEMCoaxial : public TEM {
public:
    TEMCoaxial(
            std::unique_ptr<Magnitude::Magnitude> magnitude,
            const Geometry::Element::Group<const Geometry::Surf>& elem,
            const ExcitationMode excMode,
            const Math::CVecR3& origin,
            const Math::Real innerRadius,
            const Math::Real outerRadius);
    TEMCoaxial(const TEMCoaxial& rhs);
    virtual ~TEMCoaxial() = default;

    
    void set(const Geometry::Element::Group<const Geometry::Elem>&);

    std::string getName() const { return "Coaxial_TEM_port"; }

    Math::CVecR3 getOrigin() const;
    Math::CVecR3 getWeight(const Math::CVecR3& pos) const;

private:
    Math::CVecR3 origin_;
    Math::Real innerRadius_, outerRadius_;
};

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */

