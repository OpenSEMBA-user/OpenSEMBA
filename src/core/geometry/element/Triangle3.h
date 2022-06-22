

#pragma once

#include "Triangle.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Triangle3 : public Triangle {
public:
    Triangle3(const Id id,
              const CoordR3* v[3],
              const Layer* lay = nullptr,
              const Model* mat = nullptr);
    Triangle3(const Triangle3& rhs);
    virtual ~Triangle3() = default;

    static const std::size_t sizeOfCoordinates = 3;

    virtual std::unique_ptr<Base> clone() const override {
        return std::make_unique<Triangle3>(*this);
    }

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

    void check() const;

    virtual std::unique_ptr<Element<Math::Int >> toStructured(
        const CoordI3Group&,
        const Grid3&,
        const Math::Real = Grid3::tolerance) const;

    virtual std::unique_ptr<Element<Math::Real>> toUnstructured(
        const CoordR3Group&,
        const Grid3&) const;

protected:
    static const Math::Simplex::Triangle<1> geo;

    // TODO: Remove plain array
    const CoordR3* v_[3];
};

} /* namespace Element */

typedef Element::Triangle3 Tri3;

} /* namespace Geometry */
} /* namespace SEMBA */

