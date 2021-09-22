

#pragma once

#include "OutputRequest.h"

namespace SEMBA {
namespace OutputRequest {

class BulkCurrent : public virtual OutputRequest<Geometry::Elem> {
public:
    BulkCurrent(const Domain& domain,
                const std::string& name,
                const Geometry::Element::Group<const Geometry::Elem>& box,
                const Math::Constants::CartesianAxis& dir,
                const Math::UInt& skip);
    BulkCurrent(const BulkCurrent& rhs);
    virtual ~BulkCurrent();

    SEMBA_CLASS_DEFINE_CLONE(BulkCurrent);

    Math::Constants::CartesianAxis getDir() const;
    Math::UInt                     getSkip() const;

private:
    Math::Constants::CartesianAxis dir_;
    Math::UInt                     skip_;
};

} /* namespace OutputRequest */
} /* namespace SEMBA */

