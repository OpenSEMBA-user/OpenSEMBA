#include "OnLine.h"

namespace SEMBA {
namespace Source {

OnLine::OnLine(const std::unique_ptr<Magnitude::Magnitude>& magnitude,
               const Target& elem,
               const Type& sourceType,
               const Hardness& sourceHardness)
:   Source(magnitude, elem)
{
    type_ = sourceType;
    hardness_ = sourceHardness;
}

} /* namespace Source */
} /* namespace SEMBA */
