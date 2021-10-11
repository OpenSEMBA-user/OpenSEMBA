

#include "Function.h"

#include <iostream>
#include <typeinfo>

namespace SEMBA {
namespace Math {
namespace Function {

template<class S, class T>
Function<S,T>::Function() {

}

template<class S, class T>
Function<S,T>::~Function() {

}

template<class S, class T>
inline T Function<S, T>::eval(const S& arg) const {
    return this->operator ()(arg);
}

} /* namespace Function */
} /* namespace Math */
} /* namespace SEMBA */
