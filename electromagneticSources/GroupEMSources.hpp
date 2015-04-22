/*
 * GroupEMSources.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */
#include "GroupEMSources.h"

template<typename E>
void GroupEMSources<E>::printInfo() const {
    cout << " --- GroupEMSources info --- " << endl;
    Group<E>::printInfo();
}
