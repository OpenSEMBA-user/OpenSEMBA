#pragma once

#include "Quadrilateral.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Quadrilateral4Base : public virtual SurfaceBase {
public:
    static const std::size_t sizeOfCoordinates = 4;

    Quadrilateral4Base() {}
    virtual ~Quadrilateral4Base() {}

    std::size_t numberOfCoordinates() const { return sizeOfCoordinates; }

    std::size_t numberOfSideCoordinates(const std::size_t f = 0) const { 
        return 2; 
    }
};

template<class T>
class Quadrilateral4: public virtual Quadrilateral<T>,
                      public virtual Quadrilateral4Base {
public:
    Quadrilateral4(const Id id,
                   const Coordinate::Coordinate<T,3>* coords[4],
                   const Layer* lay = nullptr,
                   const Model* mat = nullptr);
	Quadrilateral4(const Id id,
		std::array<const Coordinate::Coordinate<T, 3>*, 4> v,
		const Layer* lay,
		const Model* mat);
    Quadrilateral4(Coordinate::Group<Coordinate::Coordinate<T,3> >&,
                   const Id id,
                   const Box<T,3>& box,
                   const Layer* lay = nullptr,
                   const Model* mat = nullptr);
    Quadrilateral4(const Quadrilateral4<T>& rhs);
    virtual ~Quadrilateral4() = default;

    virtual std::unique_ptr<Base> clone() const override {
        return std::make_unique<Quadrilateral4<T>>(*this);
    }

    bool isStructured(const Grid3&, const Math::Real = Grid3::tolerance) const;

    const Coordinate::Coordinate<T,3>* getV    (const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideV(const std::size_t f,
                                                const std::size_t i) const;

    const Coordinate::Coordinate<T,3>* getVertex    (
            const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideVertex(
            const std::size_t f,
            const std::size_t i) const;

    void setV(const std::size_t i, const Coordinate::Coordinate<T,3>*);

    std::unique_ptr<ElemI> toStructured(const CoordI3Group&,
                        const Grid3&,
                        const Math::Real = Grid3::tolerance) const;
    std::unique_ptr<ElemR> toUnstructured(const CoordR3Group&,
                          const Grid3&) const;

    void check() const;

private:
    const Coordinate::Coordinate<T,3>* v_[4];
};

} /* namespace Element */

typedef Element::Quadrilateral4Base         Qua4;
typedef Element::Quadrilateral4<Math::Real> QuaR4;
typedef Element::Quadrilateral4<Math::Int > QuaI4;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Quadrilateral4.hpp"

