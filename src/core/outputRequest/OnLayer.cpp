#include "OnLayer.h"

namespace SEMBA {
namespace OutputRequest {

    OnLayer::OnLayer(
        const Type& outputType,
        const Domain& domain,
        const std::string& name,
        const Target& elem
    )
        : OutputRequest(outputType, name, domain, elem)
    {}

} /* namespace OutputRequest */
} /* namespace SEMBA */
