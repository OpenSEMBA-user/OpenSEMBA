#include "OnSurface.h"

namespace SEMBA {
namespace OutputRequest {

OnSurface::OnSurface(
    const Type& outputType,
    const Domain& domain,
    const std::string& name,
    const Target& elem
) : OutputRequest(outputType, name, domain, elem)
{}

} /* namespace OutputRequest */
} /* namespace SEMBA */
