

#pragma once

#include "math/simplex/Triangle.h"

#include "Surface.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Triangle : public Surface<Math::Real> {
public:
    virtual ~Triangle() = default;

    std::size_t numberOfFaces   () const { return 3; }
    std::size_t numberOfVertices() const { return 3; }

    std::size_t numberOfSideVertices(const std::size_t f = 0) const {
        return 2;
    }
};

} /* namespace Element */

typedef Element::Triangle Tri;
typedef std::array<CoordId, 3> TriIds;

} /* namespace Geometry */
} /* namespace SEMBA */

