#include "OnLine.h"

namespace SEMBA {
namespace Source {

OnLine::OnLine(std::unique_ptr<Magnitude::Magnitude> magnitude,
               const Geometry::Element::Group<const Geometry::Lin>& elem,
               const Type& sourceType,
               const Hardness& sourceHardness)
:   Source(magnitude, elem)
{
    type_ = sourceType;
    hardness_ = sourceHardness;
}

} /* namespace Source */
} /* namespace SEMBA */
