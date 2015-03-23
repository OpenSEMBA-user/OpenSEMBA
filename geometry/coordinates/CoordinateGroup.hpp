/*
 * CoordinateGroup.hpp
 *
 *  Created on: Aug 29, 2012
 *      Author: luis
 */
#ifndef COORDINATEGROUP_H_
#include "CoordinateGroup.h"
#endif

template<typename C>
CoordinateGroup<C>::CoordinateGroup() {
    lastPosIndex_ = 0;
}

template<typename C> template<typename C2>
CoordinateGroup<C>::CoordinateGroup(const vector<C2*>& coord)
:   GroupId<C, CoordinateId>(coord) {
    
    lastPosIndex_ = 0;
    buildIndex();
}

template<typename C>
CoordinateGroup<C>::CoordinateGroup(const vector<CVecR3>& pos) {
    lastPosIndex_ = 0;
    add(pos, true);
}

template<typename C>
CoordinateGroup<C>::CoordinateGroup(const Group<C>& rhs)
:   GroupId<C, CoordinateId>(rhs) {

    lastPosIndex_ = 0;
    buildIndex();
}

template<typename C> template<typename C2>
CoordinateGroup<C>::CoordinateGroup(const Group<C2>& rhs)
:   GroupId<C, CoordinateId>(rhs) {
    
    lastPosIndex_ = 0;
    buildIndex();
}

template<typename C>
CoordinateGroup<C>::~CoordinateGroup() {
    clearIndex();
}

template<typename C>
CoordinateGroup<C>& CoordinateGroup<C>::operator=(const Group<C>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    clearIndex();
    GroupId<C, CoordinateId>::operator=(rhs);
    buildIndex();

    return *this;
}

template<typename C> template<typename C2>
CoordinateGroup<C>& CoordinateGroup<C>::operator=(const Group<C2>& rhs) {
    clearIndex();
    GroupId<C, CoordinateId>::operator=(rhs);
    buildIndex();
    
    return *this;
}

template<typename C>
CoordinateGroup<C> CoordinateGroup<C>::get(
        const vector<CoordinateId>& ids) const {
    return GroupId<C, CoordinateId>::get(ids);
}

template<typename C>
const CoordR3* CoordinateGroup<C>::get(const CVecR3& position) const {
    multiset<const CoordR3*, lexCompareCoord>::iterator it;
    CoordR3 aux(position);
    it = indexUnstr_.find(&aux);
    if (it != indexUnstr_.end()) {
        const CoordR3* res = *it;
        return res;
    } else {
        return NULL;
    }
}

template<typename C>
const CoordI3* CoordinateGroup<C>::get(const CVecI3& position) const {
    multiset<const CoordI3*, lexCompareCoord>::iterator it;
    CoordI3 aux(position);
    it = indexStr_.find(&aux);
    if (it != indexStr_.end()) {
        const CoordI3* res = *it;
        return res;
    } else {
        return NULL;
    }
}

template<typename C> template<typename C2>
void CoordinateGroup<C>::add(C2* newCoord, bool newId) {
    vector<C2*> aux;
    aux.push_back(newCoord);
    add(aux, newId);
}

template<typename C> template<typename C2>
void CoordinateGroup<C>::add(vector<C2*>& newCoords, bool newId) {
    GroupId<C, CoordinateId>::add(newCoords, newId);
    buildIndex();
}

template<typename C> template<typename C2>
void CoordinateGroup<C>::add(const Group<C2>& rhs) {
    GroupId<C, CoordinateId>::add(rhs);
    buildIndex();
}

template<typename C>
CoordR3* CoordinateGroup<C>::add(const CVecR3& newPosition,
                                 const bool canOverlap) {
    vector<CoordR3*> res;
    vector<CVecR3> aux;
    aux.push_back(newPosition);
    res = add(aux, canOverlap);
    if(!res.empty())
        return res[0];
    return NULL;
}

template<typename C>
vector<CoordR3*> CoordinateGroup<C>::add(const vector<CVecR3>& newPos,
                                         const bool canOverlap) {
    vector<CoordR3*> res;
    res.reserve(newPos.size());

    vector<C*> newCoords;
    newCoords.reserve(newPos.size());
    for(UInt i = 0; i < newPos.size(); i++) {
        if (get(newPos[i]) == NULL || canOverlap) {
            CoordR3* newCoord = new CoordR3(newPos[i]);
            if (newCoord->template is<C>()) {
                newCoords.push_back(newCoord->template castTo<C>());
                res.push_back(newCoord);
            } else {
                delete newCoord;
            }
        }
    }
    add(newCoords, true);
    return res;
}

template<typename C>
CoordI3* CoordinateGroup<C>::add(const CVecI3& newPosition,
                                 const bool canOverlap) {
    vector<CoordI3*> res;
    vector<CVecI3> aux;
    aux.push_back(newPosition);
    res = add(aux, canOverlap);
    if(!res.empty())
        return res[0];
    return NULL;
}

template<typename C>
vector<CoordI3*> CoordinateGroup<C>::add(const vector<CVecI3>& newPos,
                                         const bool canOverlap) {
    vector<CoordI3*> res;
    res.reserve(newPos.size());

    vector<C*> newCoords;
    for(UInt i = 0; i < newPos.size(); i++) {
        if (get(newPos[i]) == NULL || canOverlap) {
            CoordI3* newCoord = new CoordI3(newPos[i]);
            res.push_back(newCoord);
            if (newCoord->template is<C>()) {
                newCoords.push_back(newCoord->template castTo<C>());
            } else {
                delete newCoord;
            }
        }
    }
    add(newCoords, true);
    return res;
}

template<typename C>
void CoordinateGroup<C>::applyScalingFactor(const Real factor) {
    for(UInt i = 0; i < this->size(); i++) {
        if (this->element_[i]->template is<CoordR3>()) {
            CoordR3* ptr = this->element_[i]->template castTo<CoordR3>();
            *ptr *= factor;
        }
    }
}

template<typename C>
void CoordinateGroup<C>::printInfo() const {
    cout<< "--- CoordinateGroup info ---" << endl;
    for (UInt i = 0; i < this->size(); i++) {
        this->element_[i]->printInfo();
        cout << endl;
    }
    cout<< "Total: " << this->size() << " coordinates." << endl;
}

template<typename C>
void CoordinateGroup<C>::buildIndex() {
    for (UInt i = lastPosIndex_; i < this->size(); i++) {
        if (this->element_[i]->template is<CoordR3>()) {
            indexUnstr_.insert(this->element_[i]->template castTo<CoordR3>());
        }
        if (this->element_[i]->template is<CoordI3>()) {
            indexStr_.insert(this->element_[i]->template castTo<CoordI3>());
        }
    }
    lastPosIndex_ = this->size();
}

template<typename C>
void CoordinateGroup<C>::clearIndex() {
    lastPosIndex_ = 0;
    indexStr_.clear();
    indexUnstr_.clear();
}
