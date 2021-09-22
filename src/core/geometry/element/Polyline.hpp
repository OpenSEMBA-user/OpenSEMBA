

#include "Polyline.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

template<class T>
Polyline<T>::Polyline() {

}

template<class T>
Polyline<T>::Polyline(const Id id,
                      const std::vector<const Coordinate::Coordinate<T,3>*>& v,
                      const Layer* lay,
                      const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {
    v_ = v;
}

template<class T>
Polyline<T>::Polyline(const Polyline<T>& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs) {
    v_ = rhs.v_;
}

template<class T>
Polyline<T>::~Polyline() {

}

template<class T>
const Coordinate::Coordinate<T,3>* Polyline<T>::getV(
        const std::size_t i) const {
    return v_[i];
}

template<class T>
const Coordinate::Coordinate<T,3>* Polyline<T>::getSideV(
        const std::size_t f,
        const std::size_t i) const {
    if (f == 0) {
        return v_.front();
    }
    return v_.back();
}

template<class T>
const Coordinate::Coordinate<T,3>* Polyline<T>::getVertex(
        const std::size_t i) const {
    if (i == 0) {
        return v_.front();
    }
    return v_.back();
}

template<class T>
const Coordinate::Coordinate<T,3>* Polyline<T>::getSideVertex(
        const std::size_t f,
        const std::size_t i) const {
    if (f == 0) {
        return v_.front();
    }
    return v_.back();
}

template<class T>
void Polyline<T>::setV(const std::size_t i,
                       const Coordinate::Coordinate<T,3>* coord) {

    assert(i < numberOfCoordinates());
    v_[i] = coord;
}

template<class T>
void Polyline<T>::printInfo() const {
    std::cout << "--- Polyline info ---" << std::endl;
    Line<T>::printInfo();
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
    }
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
