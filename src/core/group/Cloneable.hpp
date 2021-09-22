

#include "Cloneable.h"

namespace SEMBA {
namespace Group {

template<typename T>
Group<T>* Cloneable<T>::clone() const {
    return new Cloneable<T>(this->cloneElems());
}

template<typename T>
Group<typename std::remove_const<T>::type> Cloneable<T>::cloneElems() const {
    Group<typename std::remove_const<T>::type> res;
    res.reserve(this->size());
    for (std::size_t i = 0; i < this->size(); i++) {
        res.add(
            dynamic_cast<
                typename std::add_pointer<
                             typename std::remove_const<T>::type>::type>(
                                 this->get(i)->clone()));
    }
    return res;
}

} /* namespace Group */
} /* namespace SEMBA */
