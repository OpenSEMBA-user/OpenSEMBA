

#include "Polygon.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

Polygon::Polygon() {

}

Polygon::Polygon(const Id id,
                 const std::vector<const CoordR3*>& v,
                 const Layer* lay,
                 const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {
    
    const std::size_t vSize = v.size();
    assert(v.size() >= 5); // Polygons of 3 or 4 vertices are treated
                           // as triangles or quads.
    v_.resize(vSize);
    for (std::size_t i = 0; i < vSize; i++) {
        v_[i] = v[i];
    }
}

Polygon::Polygon(const Polygon& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs) {
    
    v_ = rhs.v_;
}

Polygon::~Polygon() {
    
}

std::size_t Polygon::numberOfFaces() const {
    return v_.size();
}

std::size_t Polygon::numberOfVertices() const {
    return numberOfCoordinates();
}

std::size_t Polygon::numberOfCoordinates() const {
    return v_.size();
}

std::size_t
Polygon::numberOfSideVertices(const std::size_t f = 0) const {
    return 2;
}

std::size_t
Polygon::numberOfSideCoordinates(const std::size_t f = 0) const {
    return 2;
}

const CoordR3* Polygon::getV(const std::size_t i) const {
    assert(i < numberOfCoordinates());
    return v_[i];
}

const CoordR3* Polygon::getSideV(const std::size_t f,
                                 const std::size_t i) const {
    
    return v_[(f + i) % numberOfCoordinates()];
}

const CoordR3* Polygon::getVertex(const std::size_t i) const {
    return getV(i);
}

const CoordR3* Polygon::getSideVertex(const std::size_t f,
                                      const std::size_t i) const {
    return getSideV(f,i);
}

Math::Real Polygon::getArea() const {
    throw std::logic_error("Polygon::getArea not implemented");
}

void Polygon::setV(const std::size_t i, const CoordR3* coord) {
    assert(i < numberOfCoordinates());
    v_[i] = coord;
}

void Polygon::printInfo() const {
    std::cout << "--- Polygon info ---" << std::endl
              << "Number of coordinates: " << numberOfCoordinates()
              << std::endl;
    std::cout<< "Id: " << getId() << ", MatId: " << getMatId() << std::endl;
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        std::cout<< "#" << i << ": ";
        v_[i]->printInfo();
    }
    std::cout<< "--- End of polygon info ---" << std::endl;
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
