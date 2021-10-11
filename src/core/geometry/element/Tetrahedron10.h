

#pragma once

#include "Tetrahedron.h"
#include "Tetrahedron4.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Tetrahedron10 : public Tetrahedron {
public:
    static const std::size_t sizeOfCoordinates = 10;

    Tetrahedron10();
    Tetrahedron10(const Id id,
                  const CoordR3* v[10],
                  const Layer* lay = nullptr,
                  const Model* mat = nullptr);
    Tetrahedron10(const Tetrahedron10& rhs);
    virtual ~Tetrahedron10();

    SEMBA_CLASS_DEFINE_CLONE(Tetrahedron10);

    bool isCurved() const;
    bool isQuadratic() const {return true;}
    bool isCurvedFace(const std::size_t f) const;
    bool isFaceContainedInPlane(
            const std::size_t face,
            const Math::Constants::CartesianPlane plane) const;

    std::size_t numberOfCoordinates() const { return sizeOfCoordinates; }

    std::size_t numberOfSideCoordinates(const std::size_t f = 0) const {
        return 6;
    }

    const CoordR3* getV(const std::size_t i) const {return v_[i];}
    const CoordR3* getSideV(const std::size_t f, const std::size_t i) const;

    const CoordR3* getVertex(const std::size_t i) const;
    const CoordR3* getSideVertex(const std::size_t f,
                                 const std::size_t i) const;

    Math::Real getVolume() const;
    const Math::Simplex::Simplex& getTet() const {return tet;}
    Math::Real getAreaOfFace(const std::size_t face) const;
    Triangle6 getTri6Face(const std::size_t f) const;

    void setV(const std::size_t i, const CoordR3*);

    Tetrahedron4* linearize() const;

    void check() const;

private:
    static const Math::Simplex::Triangle<2> tri;
    static const Math::Simplex::Tetrahedron<2> tet;
    const CoordR3* v_[10];
};

} /* namespace Element */

typedef Element::Tetrahedron10 Tet10;

} /* namespace Geometry */
} /* namespace SEMBA */

