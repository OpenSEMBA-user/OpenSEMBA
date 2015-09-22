/*
 * Function.cpp
 *
 *  Created on: Sep 9, 2015
 *      Author: luis
 */

#include "Function.h"

template<class S, class T>
Function<S,T>::Function() {
    // TODO Auto-generated constructor stub

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
    cout << " --- Function from " << typeid(S).name()
            << " to " << typeid(T).name() << " --- " << endl;
}
