/*
 * GroupCoordinates.hpp
 *
 *  Created on: Aug 29, 2012
 *      Author: luis
 */

#include "GroupCoordinates.h"

template<typename C>
GroupCoordinates<C>::GroupCoordinates(const vector<CVecR3>& pos) {
    GroupId<C,CoordinateId>::add(pos, true);
}

template<typename C>
GroupCoordinates<C>::GroupCoordinates(const vector<CVecI3>& pos) {
    GroupId<C,CoordinateId>::add(pos, true);
}

template<typename C>
const CoordR3* GroupCoordinates<C>::get(const CVecR3& position) const {
    multiset<const CoordR3*, lexCompareCoord>::iterator it;
    CoordR3 aux(position);
    it = indexUnstr_.find(&aux);
    if (it != indexUnstr_.end()) {
        return *it;
    } else {
        return NULL;
    }
}

template<typename C>
const CoordI3* GroupCoordinates<C>::get(const CVecI3& position) const {
    multiset<const CoordI3*, lexCompareCoord>::iterator it;
    CoordI3 aux(position);
    it = indexStr_.find(&aux);
    if (it != indexStr_.end()) {
        return *it;
    } else {
        return NULL;
    }
}

template<typename C>
C* GroupCoordinates<C>::add(const CVecR3& newPosition, const bool canOverlap) {
    vector<C*> res;
    vector<CVecR3> aux;
    aux.push_back(newPosition);
    res = add(aux, canOverlap);
    if(!res.empty()) {
        return res[0];
    }
    return NULL;
}

template<typename C>
vector<C*> GroupCoordinates<C>::add(const vector<CVecR3>& newPos,
                                   const bool canOverlap) {
    vector<C*> newCoords;
    newCoords.reserve(newPos.size());
    for(UInt i = 0; i < newPos.size(); i++) {
        if (!get(newPos[i]) || canOverlap) {
            CoordR3* newCoord = new CoordR3(newPos[i]);
            if (newCoord->template is<C>()) {
                newCoords.push_back(newCoord->castTo<C>());
            } else {
                delete newCoord;
            }
        }
    }
    return add(newCoords, true);
}

template<typename C>
C* GroupCoordinates<C>::add(const CVecI3& newPosition, const bool canOverlap) {
    vector<C*> res;
    vector<CVecI3> aux;
    aux.push_back(newPosition);
    res = add(aux, canOverlap);
    if(!res.empty()) {
        return res[0];
    }
    return NULL;
}

template<typename C>
vector<C*> GroupCoordinates<C>::add(const vector<CVecI3>& newPos,
                                              const bool canOverlap) {
    vector<C*> newCoords;
    for(UInt i = 0; i < newPos.size(); i++) {
        if (get(newPos[i]) == NULL || canOverlap) {
            CoordI3* newCoord = new CoordI3(newPos[i]);
            if (newCoord->template is<C>()) {
                newCoords.push_back(newCoord->template castTo<C>());
            } else {
                delete newCoord;
            }
        }
    }
    return add(newCoords, true);
}

template<typename C>
void GroupCoordinates<C>::applyScalingFactor(const Real factor) {
    for(UInt i = 0; i < this->size(); i++) {
        if (this->get(i)->template is<CoordR3>()) {
            CoordR3* ptr = this->get(i)->template castTo<CoordR3>();
            *ptr *= factor;
        }
    }
}

template<typename C>
void GroupCoordinates<C>::printInfo() const {
    cout<< "--- GroupCoordinates info ---" << endl;
    cout<< "Total: " << this->size() << " coordinates." << endl;
    Group<C>::printInfo();
}

template<typename C>
void GroupCoordinates<C>::construct() {
    GroupId<C, CoordinateId>::construct();
    indexStr_.clear();
    indexUnstr_.clear();
}

template<typename C>
void GroupCoordinates<C>::destruct() {
    indexStr_.clear();
    indexUnstr_.clear();
    GroupId<C, CoordinateId>::destruct();
}

template<typename C>
void GroupCoordinates<C>::postprocess(const UInt fistStep) {
    GroupId<C, CoordinateId>::postprocess(fistStep);
    for (UInt i = fistStep; i < this->size(); i++) {
        if (this->get(i)->template is<CoordR3>()) {
            indexUnstr_.insert(this->get(i)->template castTo<CoordR3>());
        }
        if (this->get(i)->template is<CoordI3>()) {
            indexStr_.insert(this->get(i)->template castTo<CoordI3>());
        }
    }
}
