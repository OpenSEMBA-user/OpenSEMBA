/*
 * GroupPhysicalModels.hpp
 *
 *  Created on: Jul 1, 2013
 *      Author: luis
 */
#include "GroupPhysicalModels.h"

template<typename P>
GroupPhysicalModels<P>& GroupPhysicalModels<P>::operator=(VectorPtr<P>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    GroupId<P, MatId>::operator=(rhs);
    return *this;
}

template<typename P>
GroupPhysicalModels<P>& GroupPhysicalModels<P>::operator=(VectorPtr<P>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    GroupId<P, MatId>::operator=(std::move(rhs));
    return *this;
}

template<typename P>
void GroupPhysicalModels<P>::printInfo() const {
    cout << "---- GroupPhysicalModels info ----" << endl;
    cout << "Number of physical models: " << this->size() << endl;
    Group<P>::printInfo();
}

template<typename P>
void GroupPhysicalModels<P>::getDirection(PMVolumePML::Direction direction[3],
                                          const UInt i) const {
    assert(i < PMVolumePML::possibleDirections);
    direction[x] = getDirectionFromInt((i/9) % 3);
    direction[y] = getDirectionFromInt((i/3) % 3);
    direction[z] = getDirectionFromInt(i % 3);
    assert( !(direction[0] == PMVolumePML::none
            && direction[1] == PMVolumePML::none
            && direction[2] == PMVolumePML::none) );
}

template<typename P>
PMVolumePML::Direction GroupPhysicalModels<P>::getDirectionFromInt(
        const UInt i) const {
    assert(PMVolumePML::plus == 0);
    assert(PMVolumePML::minus == 1);
    assert(PMVolumePML::none == 2);
    assert(i == PMVolumePML::plus
            || i == PMVolumePML::minus
            || i == PMVolumePML::none);
    if (PMVolumePML::plus == i) {
        return PMVolumePML::plus;
    } else if (PMVolumePML::minus == i) {
        return PMVolumePML::minus;
    } else {
        return PMVolumePML::none;
    }
}
