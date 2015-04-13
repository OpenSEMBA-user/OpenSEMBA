/*
 * GroupGroup.cpp
 *
 *  Created on: 5/4/2015
 *      Author: Daniel
 */

#include "GroupComp.h"

template<typename T> template<class T2>
GroupComp<typename conditional<is_const<T>::value, const T2, T2>::type>
    GroupComp<T>::getGroupOfOnly() {

    return Group<T>::getGroupWith(getElemsWith_<T2>()).template getGroupOf<T2>();
}

template<typename T> template<class T2>
GroupComp<const T2> GroupComp<T>::getGroupOfOnly() const {
    return Group<T>::getGroupWith(getElemsWith_<T2>()).template getGroupOf<T2>();
}

template<typename T> template<class T2>
GroupComp<T> GroupComp<T>::getGroupNotOfOnly() {

    return Group<T>::getGroupWithout(getElemsWith_<T2>());
}

template<typename T> template<class T2>
GroupComp<const T> GroupComp<T>::getGroupNotOfOnly() const {
    return Group<T>::getGroupWithout(getElemsWith_<T2>());
}

template<typename T> template<class T2>
vector<UInt> GroupComp<T>::getElemsWith_() const {
    vector<UInt> elems;
    elems.reserve(this->size());
    for(UInt i = 0; i < this->size(); i++) {
        if (typeid(*this->get(i)) == typeid(T2)) {
            elems.push_back(i);
        }
    }
    return elems;
}
