#pragma once

#include "math/simplex/Tetrahedron.h"
#include "Triangle3.h"
#include "Volume.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Tetrahedron : public Volume<Math::Real> {
public:
    Tetrahedron();
    virtual ~Tetrahedron();

    virtual bool isCurvedFace(const std::size_t face) const = 0;
    virtual bool isFaceContainedInPlane(const std::size_t face,
            const Math::Constants::CartesianPlane plane) const = 0;

    inline std::size_t numberOfFaces   () const { return 4; }
    inline std::size_t numberOfVertices() const { return 4; }
    inline std::size_t numberOfSideVertices(const std::size_t f = 0) const {
        return 3;
    }
    virtual const Math::Simplex::Simplex& getTet() const = 0;
    virtual Math::Real getVolume() const = 0;
    virtual Math::Real getAreaOfFace(const std::size_t face) const = 0;
    virtual Triangle3* getTri3Face(const std::size_t f) const;

};

} /* namespace Element */

typedef Element::Tetrahedron Tet;

} /* namespace Geometry */
} /* namespace SEMBA */

