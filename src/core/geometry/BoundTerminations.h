

#pragma once

#include <array>
#include <memory>
#include "geometry/element/Element.h"

namespace SEMBA {
namespace Geometry {

typedef Element::Model Bound;

template <std::size_t D>
class BoundTerminations {
public:
    BoundTerminations();
    BoundTerminations(const std::array<std::array<const Bound*, 2>, D>&);
    BoundTerminations(const BoundTerminations&);
    virtual ~BoundTerminations();

    BoundTerminations& operator=(const BoundTerminations&);

    const Bound* operator()(std::size_t d, std::size_t p) const;

    void setBound (std::size_t d, std::size_t p, const Bound*);
    void setBounds(const std::array<std::array<const Bound*, 2>, D>&);

private:
    std::array<std::array<std::shared_ptr<const Bound>, 2>, D> bounds_;
};

typedef BoundTerminations<3> BoundTerminations3;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "BoundTerminations.hpp"

