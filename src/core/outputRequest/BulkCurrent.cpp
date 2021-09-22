

#include "BulkCurrent.h"

namespace SEMBA {
namespace OutputRequest {

BulkCurrent::BulkCurrent(
        const Domain& domain,
        const std::string& name,
        const Geometry::Element::Group<const Geometry::Elem>& elem,
        const Math::Constants::CartesianAxis& dir,
        const Math::UInt& skip)
:   SEMBA::OutputRequest::Base(bulkCurrentElectric, name, domain),
    Geometry::Element::Group<const Geometry::Elem>(elem) {

    dir_ = dir;
    skip_ = skip;
}

BulkCurrent::BulkCurrent(const BulkCurrent& rhs)
:   SEMBA::OutputRequest::Base(rhs),
    Geometry::Element::Group<const Geometry::Elem>(rhs) {

    dir_ = rhs.dir_;
    skip_ = rhs.skip_;
}

BulkCurrent::~BulkCurrent() {

}

Math::Constants::CartesianAxis BulkCurrent::getDir() const {
    return dir_;
}

Math::UInt BulkCurrent::getSkip() const {
    return skip_;
}

} /* namespace OutputRequest */
} /* namespace SEMBA */
