

#pragma once

#include "source/Source.h"
#include "physicalModel/bound/Bound.h"

namespace SEMBA {
namespace Source {
namespace Port {

typedef std::array<std::array<const PhysicalModel::Bound::Bound*,2>,3> Bound3;

class Port : public Source<Geometry::Surf> {
public:

    Port(Magnitude::Magnitude* magnitude,
         const Geometry::Element::Group<const Geometry::Surf>& elem);
    Port(const Port& rhs);
    virtual ~Port();

    Math::CVecR3 getNormal() const;

    virtual Math::CVecR3 getOrigin() const = 0;
    virtual Math::CVecR3 getWeight(const Math::CVecR3& pos) const = 0;
};

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */

