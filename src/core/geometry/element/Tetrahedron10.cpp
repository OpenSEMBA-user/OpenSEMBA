

#include "Tetrahedron10.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

const Math::Simplex::Triangle<2>    Tetrahedron10::tri;
const Math::Simplex::Tetrahedron<2> Tetrahedron10::tet;

Tetrahedron10::Tetrahedron10() {

}

Tetrahedron10::Tetrahedron10(const Id id,
                             const CoordR3* v[10],
                             const Layer* lay,
                             const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {

    for (std::size_t i = 0; i < tet.np; i++) {
        v_[i] = v[i];
    }
    //
    check();
}

Tetrahedron10::Tetrahedron10(const Tetrahedron10& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs) {

    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Tetrahedron10::~Tetrahedron10() {

}

bool Tetrahedron10::isCurved() const {
    for (std::size_t f = 0; f < tet.faces; f++) {
        if(isCurvedFace(f)) {
            return true;
        }
    }
    return false;
}

bool Tetrahedron10::isCurvedFace(const std::size_t f) const {
    return getTri6Face(f).isCurved();
}

bool
Tetrahedron10::isFaceContainedInPlane(
        const std::size_t face,
        const Math::Constants::CartesianPlane plane) const {
    return getTri6Face(face).isContainedInPlane(plane);
}

const CoordR3* Tetrahedron10::getSideV(const std::size_t f,
                                       const std::size_t i) const {
    return v_[tet.sideNode(f,i)];
}

const CoordR3*
Tetrahedron10::getVertex(const std::size_t i) const {
    return v_[tet.vertex(i)];
}

const CoordR3* Tetrahedron10::getSideVertex(const std::size_t f,
                                            const std::size_t i) const {
    return v_[tet.sideVertex(f, i)];
}

Math::Real Tetrahedron10::getVolume() const {
    throw std::logic_error("Get volume not implemented");
}

Math::Real Tetrahedron10::getAreaOfFace(const std::size_t f) const {
    return getTri6Face(f).getArea();
}

Triangle6 Tetrahedron10::getTri6Face(const std::size_t f) const {
    const CoordR3* sideV[6];
    for (std::size_t i = 0; i < 6; i++) {
        sideV[i] = getSideV(f,i);
    }
    Triangle6 auxFace(Id(0), sideV);
    return auxFace;
}

void Tetrahedron10::setV(const std::size_t i, const CoordR3* vNew) {
    v_[i] = vNew;
}

Tetrahedron4* Tetrahedron10::linearize() const {
    const CoordR3* vertex[4];
    for (std::size_t i = 0; i < 4; i++) {
        vertex[i] = getVertex(i);
    }
    return new Tetrahedron4(getId(), vertex, getLayer(), getModel());
}

void Tetrahedron10::printInfo() const {
    std::cout << "--- Tet10 info ---" << std::endl;
    std::cout << "Id: " << getId() << std::endl;
    std::cout << "Coordinates:" << std::endl;
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
    }
}

void Tetrahedron10::check() const {

}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
