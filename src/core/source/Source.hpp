

#include "Source.h"

namespace SEMBA {
namespace Source {

template <class T>
void Source<T>::set(
        const Geometry::Element::Group<const Geometry::Elem>& elems) {
    Geometry::Element::Group<const T>::operator=(elems);
}

template <class T>
void Source<T>::add(
        const Geometry::Element::Group<const Geometry::Elem>& elems) {
    Geometry::Element::Group<const T>::add(elems);
}

template<class T>
void Source<T>::printInfo() const {
    Base::printInfo();
    this->Geometry::Element::Group<const T>::printInfo();
}

} /* namespace Source */
} /* namespace SEMBA */
