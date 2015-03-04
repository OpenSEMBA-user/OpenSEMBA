/*
 * PhysicalModelGroup.cpp
 *
 *  Created on: Jul 1, 2013
 *      Author: luis
 */

#ifndef PHYSICALMODELGROUP_H_
#include "PhysicalModelGroup.h"
#endif

PhysicalModelGroup::PhysicalModelGroup() {
}

PhysicalModelGroup::~PhysicalModelGroup() {
}

//GroupWithIdBase<PMVolumePML,uint> getAssignedPMLs() const {
//    GroupWithIdBase<PMVolumePML,uint> pmls, res;
//    pmls = this->getGroupOf();
//    for (uint i = 0; i < pmls.size(); i++) {
//        if (pmls(i)->getOrientation() != PMVolumePML::undefined) {
//            res.add(pmls(i));
//        }
//    }
//    return res;
//}

void
PhysicalModelGroup::printInfo() const {
    cout << "---- Physical Models ----" << endl;
    cout << "Number of physical models: " << size() << endl;
    for (uint i = 0; i < size(); i++) {
        element_[i]->printInfo();
    }
}


void
PhysicalModelGroup::getDirection(
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

PMVolumePML::Direction
PhysicalModelGroup::getDirectionFromInt(
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

vector<uint>
PhysicalModelGroup::getIds(const Condition::Type type) const {
    vector<uint> res;
    for (uint i = 0; i < size(); i++) {
        if (element_[i]->getConditionType() == type || type == Condition::undefined) {
            res.push_back(element_[i]->getId());
        }
    }
    return res;
}


inline bool PhysicalModelGroup::hasPEC() const {
    for (uint i = 0; i < size(); i++) {
        if (element_[i]->isPEC()) {
            return true;
        }
    }
    return false;
}

inline uint PhysicalModelGroup::getPECId() const {
    for (uint i = 0; i < size(); i++) {
        if (element_[i]->isPEC()) {
            return element_[i]->getId();
        }
    }
    return 0;
}

inline uint PhysicalModelGroup::countSIBC() const {
    uint res = 0;
    for (uint i = 0; i < size(); i++) {
        if (element_[i]->isSurfaceImpedance()) {
            res++;
        }
    }
    return res;
}

inline uint PhysicalModelGroup::countMultilayers() const {
    uint res = 0;
    for (uint i = 0; i < size(); i++) {
        if (element_[i]->isSurfaceMultilayer()) {
            res++;
        }
    }
    return res;
}

inline uint PhysicalModelGroup::countClassic() const {
    uint res = 0;
    for (uint i = 0; i < size(); i++) {
        if (element_[i]->isClassic()) {
            res++;
        }
    }
    return res;
}

inline uint PhysicalModelGroup::countDispersiveVolumic() const {
    uint res = 0;
    for (uint i = 0; i < size(); i++) {
        if (element_[i]->isDispersive()) {
            res++;
        }
    }
    return res;
}
