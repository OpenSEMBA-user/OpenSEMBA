#include "Port.h"

namespace SEMBA {
namespace Source {
namespace Port {

Port::Port(const std::unique_ptr<Magnitude::Magnitude>& magnitude,
           const Target& elem) :   
    Source(magnitude, elem) 
{}

Math::CVecR3 Port::getNormal() const 
{
    /*Geometry::ElemView target = this->target_;
    target.at(0)->cas*/

    auto surf = this->target_.at(0)->castTo<Geometry::Surf>();
    //auto surf = this->target_.begin()->get()->castTo<Geometry::Surf>();
    assert(surf != nullptr);
    return surf->getNormal();
}

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */

