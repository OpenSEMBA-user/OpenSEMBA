/*
 * EMSourceGroup.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */
#include "EMSourceGroup.h"

template<typename E>
void EMSourceGroup<E>::printInfo() const {
    cout << " --- EMSourceGroup info --- " << endl;
    Group<E>::printInfo();
}
