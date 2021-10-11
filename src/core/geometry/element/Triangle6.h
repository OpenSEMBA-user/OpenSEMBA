

#pragma once

#include "Triangle.h"
#include "Triangle3.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Triangle6 : public Triangle {
public:
    Triangle6();
    Triangle6(const Id id,
              const CoordR3*[6],
              const Layer* lay = nullptr,
              const Model* mat = nullptr);
    Triangle6(const Triangle6& rhs);
    virtual ~Triangle6();

    SEMBA_CLASS_DEFINE_CLONE(Triangle6);

    bool isCurved   () const;
    bool isQuadratic() const { return true; }

    std::size_t numberOfCoordinates() const { return 6; }

    std::size_t numberOfSideCoordinates(const std::size_t f = 0) const {
        return 3;
    }

    const CoordR3* getV(const std::size_t i) const {return v_[i];}
    const CoordR3* getSideV(const std::size_t face, const std::size_t i) const;

    const CoordR3* getVertex(const std::size_t i) const;
    const CoordR3* getSideVertex(const std::size_t face,
                                 const std::size_t i) const;

    Math::Real getArea() const;

    void setV(const std::size_t i, const CoordR3*);

    Triangle3* linearize() const;

protected:
    static const Math::Simplex::Triangle<2> geo;
    const CoordR3* v_[6];
};

} /* namespace Element */

typedef Element::Triangle6 Tri6;

} /* namespace Geometry */
} /* namespace SEMBA */

