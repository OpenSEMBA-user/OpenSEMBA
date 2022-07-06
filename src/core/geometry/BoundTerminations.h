

#pragma once

#include <array>
#include <memory>
#include "geometry/element/Element.h"
#include "physicalModel/Bound.h"

namespace SEMBA {
namespace Geometry {

template <std::size_t D>
class BoundTerminations {
public:
    BoundTerminations();
    BoundTerminations(const std::array<std::array<std::unique_ptr<PhysicalModel::Bound>, 2>, D>&);
    BoundTerminations(const BoundTerminations&);
    virtual ~BoundTerminations() = default;

    BoundTerminations& operator=(const BoundTerminations&);

    const PhysicalModel::Bound* operator()(std::size_t d, std::size_t p) const;

    void setBound (std::size_t d, std::size_t p, std::unique_ptr<PhysicalModel::Bound>&&);
    void setBounds(const std::array<std::array<std::unique_ptr<PhysicalModel::Bound>, 2>, D>&);

private:
    std::array<std::array<std::unique_ptr<PhysicalModel::Bound>, 2>, D> bounds_;
};

typedef BoundTerminations<3> BoundTerminations3;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "BoundTerminations.hpp"

