

#include "Volume.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

template<class T>
Volume<T>::Volume() {

}

template<class T>
Volume<T>::~Volume() {

}

template<class T>
bool Volume<T>::isLocalFace(const std::size_t f,
                            const Surface<T>& surf) const {
    return getSideNormal(f) == surf.getNormal();
}

template<class T>
bool Volume<T>::isFaceContainedInPlane(
        const std::size_t face,
        const Math::Constants::CartesianPlane plane) const {
   Box<T,3> box = getBoundOfFace(face);
   Math::Vector::Cartesian<T,3> vec = box.getMax() - box.getMin();
   return vec.isContainedInPlane(plane);
}

template<class T>
Math::Vector::Cartesian<T,3> Volume<T>::getSideNormal(
        const std::size_t f) const {
    Math::Vector::Cartesian<T,3> vec1, vec2, res;
    vec1 = *this->getSideVertex(f,1) - *this->getSideVertex(f,0);
    vec2 = *this->getSideVertex(f,2) - *this->getSideVertex(f,0);
    res = (vec1 ^ vec2).normalize();
    return res;
}

template<class T>
Box<T,3> Volume<T>::getBoundOfFace(const std::size_t face) const {
    Box<T,3> res;
   for (std::size_t i = 0; i < this->numberOfSideCoordinates(); i++) {
      res << this->getSideV(face,i)->pos();
   }
   return res;
}

template<class T>
std::size_t Volume<T>::getFaceNumber(const Surface<T>* surf) const {
    // Checks each face. Order is not important.
    for (std::size_t f = 0; f < this->numberOfFaces(); f++) {
        std::size_t vPresent = 0;
        for (std::size_t i = 0; i < surf->numberOfVertices(); i++) {
            for (std::size_t j = 0; j < surf->numberOfVertices(); j++) {
                if (surf->getVertex(j) == this->getSideVertex(f, i)) {
                    vPresent++;
                }
            }
            if (vPresent == surf->numberOfVertices()) {
                return f;
            }
        }
    }
    throw Error::SurfNotFound(surf->getId(), this->getId());
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
