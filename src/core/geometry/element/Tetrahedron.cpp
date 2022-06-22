

#include "Tetrahedron.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

Tetrahedron::Tetrahedron() {

}

Tetrahedron::~Tetrahedron() {

}

Triangle3* Tetrahedron::getTri3Face(const std::size_t f) const {
    // TODO: Remove plain array
    const CoordR3* coord[3];
    for (std::size_t i = 0; i < 3; i++) {
        coord[i] = getSideVertex(f,i);
    }
    return new Triangle3(Id(0), coord, getLayer(), getModel());
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
