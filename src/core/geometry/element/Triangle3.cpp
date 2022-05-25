

#include "Triangle3.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

const Math::Simplex::Triangle<1> Triangle3::geo;

Triangle3::Triangle3(const Id id,
           const CoordR3* v[3],
           const Layer* lay,
           const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {
    for (std::size_t i = 0; i < geo.np; i++) {
        v_[i] = v[i];
    }
    check();
}

Triangle3::Triangle3(const Triangle3& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs) {
    
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

const CoordR3* Triangle3::getSideV(const std::size_t f,
                                   const std::size_t i) const {
    assert(f < numberOfFaces());
    assert(i < numberOfSideCoordinates());
    return v_[geo.sideNode(f, i)];
}

const CoordR3* Triangle3::getVertex(const std::size_t i) const {
    return v_[geo.vertex(i)];
}

const CoordR3* Triangle3::getSideVertex(const std::size_t f,
                                        const std::size_t i) const {
    assert(f < numberOfFaces());
    assert(i < numberOfSideVertices());
    return v_[geo.sideVertex(f, i)];
}

Math::Real Triangle3::getArea() const {
    Math::CVecR3 v1, v2;
    v1 = getVertex(1)->pos() - getVertex(0)->pos();
    v2 = getVertex(2)->pos() - getVertex(0)->pos();
    return ((Math::Real) 0.5 * (v1 ^ v2).norm());
}

void Triangle3::setV(const std::size_t i, const CoordR3* vNew) {
    v_[i] = vNew;
}

void Triangle3::check() const {

}

std::unique_ptr<ElemI> Triangle3::toStructured(
    const CoordI3Group& cG,
    const Grid3& grid, const Math::Real tol) const {

    throw std::logic_error("Triangle3::toStructured operation not permitted");
}

std::unique_ptr<ElemR> Triangle3::toUnstructured(
    const CoordR3Group& cG,
    const Grid3& grid) const {
    return std::make_unique<Tri3>(this->getId(),
        this->vertexToUnstructured(cG, grid).data(),
        this->getLayer(),
        this->getModel());
}


} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
