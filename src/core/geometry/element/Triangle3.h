

#pragma once

#include "Triangle.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Triangle3 : public Triangle {
public:
    Triangle3();
    Triangle3(const Id id,
              const CoordR3* v[3],
              const Layer* lay = nullptr,
              const Model* mat = nullptr);
    Triangle3(const Triangle3& rhs);
    virtual ~Triangle3();

    static const std::size_t sizeOfCoordinates = 3;

    SEMBA_CLASS_DEFINE_CLONE(Triangle3);

    std::size_t numberOfCoordinates() const { return sizeOfCoordinates; }

    std::size_t numberOfSideCoordinates(const std::size_t f = 0) const {
        return 2;
    }

    const CoordR3* getV     (const std::size_t i) const { return v_[i]; }
    const CoordR3* getVertex(const std::size_t i) const;

    const CoordR3* getSideV     (const std::size_t face,
                                 const std::size_t i) const;
    const CoordR3* getSideVertex(const std::size_t face,
                                 const std::size_t i) const;

    Math::Real getArea() const;

    void setV(const std::size_t i, const CoordR3*);

    Triangle3* linearize() const;

    void printInfo() const;
    void check() const;

protected:
    static const Math::Simplex::Triangle<1> geo;
    const CoordR3* v_[3];
};

} /* namespace Element */

typedef Element::Triangle3 Tri3;

} /* namespace Geometry */
} /* namespace SEMBA */

