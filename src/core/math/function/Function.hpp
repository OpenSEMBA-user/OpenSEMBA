

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

template<class S, class T>
inline void Function<S, T>::printInfo() const {
    std::cout << " --- Function from " << typeid(S).name()
              << " to " << typeid(T).name() << " --- " << std::endl;
}

} /* namespace Function */
} /* namespace Math */
} /* namespace SEMBA */
