#include "Port.h"

namespace SEMBA {
namespace Source {
namespace Port {

Port::Port(const std::unique_ptr<Magnitude::Magnitude>& magnitude,
           const Geometry::Element::Group<Geometry::Surf>& elem) :   
    Source(magnitude, elem) 
{
}

Math::CVecR3 Port::getNormal() const {
    if (this->target_.getOf<Geometry::Surf>().size() > 0) {
        if (this->get(0)->is<Geometry::SurfR>()) {
            return this->get(0)->castTo<Geometry::SurfR>()->getNormal();
        } else {
            Math::CVecI3 aux =
                this->get(0)->castTo<Geometry::SurfI>()->getNormal();
            Math::CVecR3 res;
            for (Math::UInt d = 0; d < 3; d++) {
                res(d) = (Math::Real) aux(d);
            }
            return res;
        }
    }
    return Math::CVecR3();
}

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */

