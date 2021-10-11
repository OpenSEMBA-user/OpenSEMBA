

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

} /* namespace Source */
} /* namespace SEMBA */
