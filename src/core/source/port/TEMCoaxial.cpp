#include "TEMCoaxial.h"
#include "geometry/Bound.h"

namespace SEMBA {
namespace Source {
namespace Port {

TEMCoaxial::TEMCoaxial(
        const std::unique_ptr<Magnitude::Magnitude>& magnitude,
        const Target& elem,
        const ExcitationMode excMode,
        const Math::CVecR3& origin,
        const Math::Real innerRadius,
        const Math::Real outerRadius) :
        TEM(magnitude, elem, excMode) 
{
    origin_ = origin;
    innerRadius_ = innerRadius;
    outerRadius_ = outerRadius;
}

TEMCoaxial::TEMCoaxial(const TEMCoaxial& rhs) : 
    TEM(rhs) 
{
    origin_ = rhs.origin_;
    innerRadius_ = rhs.innerRadius_;
    outerRadius_ = rhs.outerRadius_;
}

Math::CVecR3 TEMCoaxial::getOrigin() const {
    return origin_;
}

Math::CVecR3 TEMCoaxial::getWeight(
        const Math::CVecR3& pos) const {
    // Return normalized weights for electric field components.
    const Math::Real rho = (pos - getOrigin()).norm();
    switch (getExcitationMode()) {
    case ExcitationMode::voltage:
    {
        const Math::CVecR3 rhoHat = (pos - getOrigin()).normalize();
        return rhoHat / (rho * log(outerRadius_/innerRadius_));
    }
    case ExcitationMode::current:
    {
        const Math::CVecR3 phiHat = (Math::CVecR3(0,0,1) ^ pos).normalize();
        return phiHat / (2.0 * Math::Constants::pi * rho);
    }
    default:
        throw std::logic_error("Unsupported excitation mode.");
    }
}

void TEMCoaxial::set(const Target& elemGroup) {
    // Reescales internal dimensions.
    //Geometry::BoxR3 box = elemGroup.getBound();
    Geometry::BoxR3 box = Geometry::getBound(elemGroup.begin(), elemGroup.end());
    const Math::CVecR3 diagonal = box.getMax()-box.getMin();
    if (!diagonal.isContainedInPlane(Math::Constants::CartesianPlane::xy)) {
        throw std::logic_error("Port is not contained in a XY plane");
    }
    const Math::Real width  = box.getMax()(Math::Constants::x) -
                              box.getMin()(Math::Constants::x);
    const Math::Real height = box.getMax()(Math::Constants::y) -
                              box.getMin()(Math::Constants::y);
    const Math::Real averageNewRadius = (width + height)/4;
    innerRadius_ *= averageNewRadius;
    outerRadius_ *= averageNewRadius;
    const Math::CVecR3 averageNewOrigin = (box.getMax() + box.getMin()) / 2;
    origin_ = averageNewOrigin;
    //
    Source::setTarget(elemGroup);
}

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */
