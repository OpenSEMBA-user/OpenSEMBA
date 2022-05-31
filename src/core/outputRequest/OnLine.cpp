#include "OnLine.h"

namespace SEMBA {
namespace OutputRequest {
    OnLine::OnLine(
        const Type& outputType,
        const Domain& domain,
        const std::string& name,
        const Target& elem
    )
        : OutputRequest(outputType, name, domain, elem)
    {}

} /* namespace OutputRequest */
} /* namespace SEMBA */
