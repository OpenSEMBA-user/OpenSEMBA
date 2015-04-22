/*
 * EMSourceGroup.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */
#include "EMSourceGroup.h"

template<typename E>
bool EMSourceGroup<E>::isSimilar(const EMSourceGroup& rhs) const {
    if (this->size() != rhs.size()) {
        return false;
    }
    for (UInt i = 0; i < this->size(); i++) {
        if (!this->get(i)->isSimilar(*rhs(i))) {
            return false;
        }
    }
    return true;
}

template<typename E>
bool EMSourceGroup<E>::check() const {
    for (UInt i = 0; i < this->size(); i++) {
        if (!this->get(i)->check()) {
            return false;
        }
    }
    return true;
}

template<typename E>
void EMSourceGroup<E>::printInfo() const {
    cout << " --- EMSourceGroup info --- " << endl;
    Group<E>::printInfo();
}

