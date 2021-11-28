#include "Port.h"

namespace SEMBA {
namespace Source {
namespace Port {

Port::Port(const std::unique_ptr<Magnitude::Magnitude>& magnitude,
           const Geometry::Element::Group<Geometry::Surf>& elem) :   
    Source(magnitude, elem) 
{}

Math::CVecR3 Port::getNormal() const 
{
    auto surf = this->target_.begin()->get()->castTo<Geometry::Surf>();
    assert(surf != nullptr);
    return surf->getNormal();
}

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */

