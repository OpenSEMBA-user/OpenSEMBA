/*
 * PhysicalModelGroup.cpp
 *
 *  Created on: Jul 1, 2013
 *      Author: luis
 */

#ifndef PHYSICALMODELGROUP_H_
#include "PhysicalModelGroup.h"
#endif

template<typename P = PhysicalModel>
PhysicalModelGroup<P>::PhysicalModelGroup() {
}

template<typename P = PhysicalModel>
PhysicalModelGroup<P>::~PhysicalModelGroup() {
}

template<typename P = PhysicalModel>
void
PhysicalModelGroup<P>::printInfo() const {
    cout << "---- Physical Models ----" << endl;
    cout << "Number of physical models: " << size() << endl;
    for (uint i = 0; i < size(); i++) {
        element_[i]->printInfo();
    }
}


template<typename P = PhysicalModel>
void
PhysicalModelGroup<P>::getDirection(
        PMVolumePML::Direction direction[3],
        const uint i) const {
    assert(i < PMVolumePML::possibleDirections);
    direction[x] = getDirectionFromInt((i/9) % 3);
    direction[y] = getDirectionFromInt((i/3) % 3);
    direction[z] = getDirectionFromInt(i % 3);
    assert( !(direction[0] == PMVolumePML::none
            && direction[1] == PMVolumePML::none
            && direction[2] == PMVolumePML::none) );
}

template<typename P = PhysicalModel>
PMVolumePML::Direction
PhysicalModelGroup<P>::getDirectionFromInt(
        const uint i) const {
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

template<typename P = PhysicalModel>
vector<uint>
PhysicalModelGroup<P>::getIds(const Condition::Type type) const {
    vector<uint> res;
    for (uint i = 0; i < size(); i++) {
        if (element_[i]->getConditionType() == type || type == Condition::undefined) {
            res.push_back(element_[i]->getId());
        }
    }
    return res;
}


template<typename P = PhysicalModel>
inline bool
PhysicalModelGroup<P>::hasPEC() const {
    for (uint i = 0; i < size(); i++) {
        if (element_[i]->isPEC()) {
            return true;
        }
    }
    return false;
}

template<typename P = PhysicalModel>
inline uint
PhysicalModelGroup<P>::getPECId() const {
    for (uint i = 0; i < size(); i++) {
        if (element_[i]->isPEC()) {
            return element_[i]->getId();
        }
    }
    return 0;
}

template<typename P = PhysicalModel>
inline uint
PhysicalModelGroup<P>::countSIBC() const {
    uint res = 0;
    for (uint i = 0; i < size(); i++) {
        if (element_[i]->isSurfaceImpedance()) {
            res++;
        }
    }
    return res;
}

template<typename P = PhysicalModel>
inline uint
PhysicalModelGroup<P>::countMultilayers() const {
    uint res = 0;
    for (uint i = 0; i < size(); i++) {
        if (element_[i]->isSurfaceMultilayer()) {
            res++;
        }
    }
    return res;
}

template<typename P = PhysicalModel>
inline uint
PhysicalModelGroup<P>::countClassic() const {
    uint res = 0;
    for (uint i = 0; i < size(); i++) {
        if (element_[i]->isClassic()) {
            res++;
        }
    }
    return res;
}

template<typename P = PhysicalModel>
inline uint
PhysicalModelGroup<P>::countDispersiveVolumic() const {
    uint res = 0;
    for (uint i = 0; i < size(); i++) {
        if (element_[i]->isDispersive()) {
            res++;
        }
    }
    return res;
}
