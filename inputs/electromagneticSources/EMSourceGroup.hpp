/*
 * EMSourceGroup.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */
#include "EMSourceGroup.h"

template<typename E>
EMSourceGroup<E>::EMSourceGroup() {

}

template<typename E> template<typename E2>
EMSourceGroup<E>::EMSourceGroup(const vector<E2*>& coord)
:   Group<E>(coord) {

}

template<typename E>
EMSourceGroup<E>::EMSourceGroup(const Group<E>& rhs)
:   Group<E>(rhs) {

}

template<typename E> template<typename E2>
EMSourceGroup<E>::EMSourceGroup(const Group<E2>& rhs)
:   Group<E>(rhs) {

}

template<typename E>
EMSourceGroup<E>::~EMSourceGroup() {

}

template<typename E>
EMSourceGroup<E>& EMSourceGroup<E>::operator=(const Group<E>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    Group<E>::operator=(rhs);

    return *this;
}

template<typename E> template<typename E2>
EMSourceGroup<E>& EMSourceGroup<E>::operator=(const Group<E2>& rhs) {
    Group<E>::operator=(rhs);

    return *this;
}

template<typename E>
void EMSourceGroup<E>::printInfo() const {
    cout << " --- EMSourceGroup info --- " << endl;
    for (UInt i = 0; i < this->size(); i++) {
        this->element_[i]->printInfo();
    }
}
