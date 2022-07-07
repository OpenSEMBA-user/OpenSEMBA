#pragma once

#include <array>

#include "Volume.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Hexahedron8Base : public virtual VolumeBase {
public:
    static const std::size_t sizeOfCoordinates = 8;

    virtual ~Hexahedron8Base() = default;

    inline bool isQuadratic() const { return false; }

    inline std::size_t numberOfFaces      () const { return 6; }
    inline std::size_t numberOfVertices   () const { return 8; }
    inline std::size_t numberOfCoordinates() const { return sizeOfCoordinates; }

    inline std::size_t numberOfSideVertices   (const std::size_t f = 0) const {
        return 4;
    }
    inline std::size_t numberOfSideCoordinates(const std::size_t f = 0) const {
        return 4;
    }
};

template<class T>
class Hexahedron8 : public virtual Volume<T>,
              public virtual Hexahedron8Base {
public:
    Hexahedron8(const Id id,
                const Coordinate::Coordinate<T,3>* v[8],
				const Layer* lay = nullptr,
                const Model* mat = nullptr);
    Hexahedron8(Coordinate::Group<Coordinate::Coordinate<T,3> >&,
                const Id id,
                const Box<T,3>& box,
                const Layer* lay = nullptr,
                const Model* mat = nullptr);
    Hexahedron8(const Hexahedron8<T>& rhs);
    virtual ~Hexahedron8() = default;

    virtual std::unique_ptr<Base> clone() const override {
        return std::make_unique<Hexahedron8>(*this);
    }

    bool isStructured(const Grid3&, const Math::Real = Grid3::tolerance) const;

    // Checks if edges are ortonormal.
    bool isRegular() const;
    inline bool isCurvedFace(const std::size_t f) const { return false; }

    const Coordinate::Coordinate<T,3>* getV    (const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideV(const std::size_t f,
                                                const std::size_t i) const;

    const Coordinate::Coordinate<T,3>* getVertex    (
            const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideVertex(
            const std::size_t f,
            const std::size_t i) const;

    std::vector<const Coordinate::Coordinate<T,3>*> getVertices() const;
    std::vector<const Coordinate::Coordinate<T,3>*> getSideVertices(
            const std::size_t face) const;

    Math::Real getAreaOfFace(const std::size_t face) const;
    Math::Real getVolume() const;

    void setV(const std::size_t i, const Coordinate::Coordinate<T,3>*);

    std::unique_ptr<ElemI> toStructured(const Coordinate::Group<CoordI3>&,
                        const Grid3&,
                        const Math::Real = Grid3::tolerance) const;
    std::unique_ptr<ElemR> toUnstructured(const Coordinate::Group<CoordR3>&,
                          const Grid3&) const;

private:
    std::array<const Coordinate::Coordinate<T,3>*, 8> v_;

    const static Math::Real tolerance;
};

} /* namespace Element */

typedef Element::Hexahedron8Base         Hex8;
typedef Element::Hexahedron8<Math::Real> HexR8;
typedef Element::Hexahedron8<Math::Int > HexI8;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Hexahedron8.hpp"

