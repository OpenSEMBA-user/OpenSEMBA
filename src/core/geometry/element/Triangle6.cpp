

#include "Triangle6.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

const Math::Simplex::Triangle<2> Triangle6::geo;

Triangle6::Triangle6() {

}

Triangle6::Triangle6(const Id id,
                     const CoordR3* v[6],
                     const Layer* lay,
                     const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {

    for (std::size_t i = 0; i < geo.np; i++) {
        v_[i] = v[i];
    }
}

Triangle6::Triangle6(const Triangle6& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs) {

    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Triangle6::~Triangle6() {

}

bool Triangle6::isCurved() const {
    for (std::size_t s = 0; s < geo.faces; s++) {
        Math::CVecR3 midPoint =
                (getSideV(s,2)->pos() + getSideV(s,0)->pos())
                / (Math::Real) 2.0;
        if(midPoint != getSideV(s,1)->pos()) {
            return true;
        }
    }
    return false;
}

const CoordR3* Triangle6::getSideV(const std::size_t face,
                                   const std::size_t i) const {
    assert(face < numberOfFaces());
    assert(i < numberOfSideCoordinates());
    return v_[geo.sideNode(face,i)];
}

const CoordR3* Triangle6::getVertex(const std::size_t i) const {
    return v_[geo.vertex(i)];
}

const CoordR3* Triangle6::getSideVertex(const std::size_t face,
                                        const std::size_t i) const {
    assert(face < numberOfFaces());
    assert(i < numberOfSideVertices());
    return v_[geo.sideVertex(face,i)];
}

Math::Real Triangle6::getArea() const {
    throw std::logic_error("Triangle6::getArea()");
}

void Triangle6::setV(const std::size_t i, const CoordR3* vNew) {
    v_[i] = vNew;
}

Triangle3* Triangle6::linearize() const {
    const CoordR3* vertex[3];
    for (std::size_t i = 0; i < 3; i++) {
        vertex[i] = getVertex(i);
    }
    return new Triangle3(getId(), vertex, getLayer(), getModel());
}

void Triangle6::printInfo() const {
    std::cout << "--- Tri6 info ---" << std::endl;
    std::cout << "Id: " << getId() << "  Mat Id: " << getMatId() << std::endl;
    std::cout << "Coordinates:" << std::endl;
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
    }
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
