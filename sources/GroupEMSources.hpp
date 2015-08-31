/*
 * GroupEMSources.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */
#include "../sources/GroupEMSources.h"

template<typename E>
GroupEMSources<E>& GroupEMSources<E>::operator=(VectorPtr<E>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    Group<E>::operator=(rhs);
    return *this;
}

template<typename E>
GroupEMSources<E>& GroupEMSources<E>::operator=(VectorPtr<E>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    Group<E>::operator=(std::move(rhs));
    return *this;
}

template<typename E>
bool GroupEMSources<E>::isSimilar(const GroupEMSources& rhs) const {
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
bool GroupEMSources<E>::check() const {
    for (UInt i = 0; i < this->size(); i++) {
        if (!this->get(i)->check()) {
            return false;
        }
    }
    return true;
}

template<typename E>
void GroupEMSources<E>::printInfo() const {
    cout << " --- EMSourceGroup info --- " << endl;
    Group<E>::printInfo();
}
