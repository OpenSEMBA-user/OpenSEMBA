

#include "BulkCurrent.h"

namespace SEMBA {
namespace OutputRequest {

BulkCurrent::BulkCurrent(
        const Domain& domain,
        const std::string& name,
        const Target& elem,
        const Math::Constants::CartesianAxis& dir,
        const Math::UInt& skip) :
    OutputRequest(Type::bulkCurrentElectric, name, domain, elem)
{
    dir_ = dir;
    skip_ = skip;
}

Math::Constants::CartesianAxis BulkCurrent::getDir() const {
    return dir_;
}

Math::UInt BulkCurrent::getSkip() const {
    return skip_;
}

} /* namespace OutputRequest */
} /* namespace SEMBA */
