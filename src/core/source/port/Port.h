#pragma once

#include "source/Source.h"
#include "physicalModel/Bound.h"

namespace SEMBA {
namespace Source {
namespace Port {

typedef std::array<std::array<const PhysicalModel::Bound*,2>,3> Bound3;

class Port : public Source {
public:
    Port(const std::unique_ptr<Magnitude::Magnitude>& magnitude, 
         const Target& elem);
    virtual ~Port() = default;

    //virtual std::unique_ptr<Source> clone() const override {
    //    return std::make_unique<Port>(*this);
    //}

    std::string getName() const { return "Port"; };

    Math::CVecR3 getNormal() const;

    virtual Math::CVecR3 getOrigin() const = 0;
    virtual Math::CVecR3 getWeight(const Math::CVecR3& pos) const = 0;
};

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */

