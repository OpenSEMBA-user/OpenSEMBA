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

}

template<typename C>
CoordinateGroup<C>::CoordinateGroup(const vector<C*>& coord)
:   GroupId<C, CoordinateId>(coord) {
    
    buildIndex(this->element_);
}

template<typename C>
CoordinateGroup<C>::CoordinateGroup(const vector<CVecD3>& pos) {
    add(pos, true);
}

template<typename C>
CoordinateGroup<C>::CoordinateGroup(const Group<C>& rhs)
:   GroupId<C, CoordinateId>(rhs) {
    
    buildIndex(this->element_);
}

template<typename C>
CoordinateGroup<C>::~CoordinateGroup() {

}

template<typename C>
CoordinateGroup<C>& CoordinateGroup<C>::operator=(const Group<C>& rhs) {
    
    if (this == &rhs) {
        return *this;
    }
    
    GroupId<C, CoordinateId>::operator=(rhs);
    buildIndex(this->element_);
    
    return *this;
}

template<typename C>
const CoordD3* CoordinateGroup<C>::get(const CVecD3& position) const {
    multiset<const CoordD3*, lexCompareCoord>::iterator it;
    CoordD3 aux(position);
    it = index_.find(&aux);
    if (it != index_.end()) {
        const CoordD3* res = *it;
        return res;
    } else {
        return NULL;
    }
}

template<typename C>
void CoordinateGroup<C>::add(C* newCoord, bool newId) {
    return GroupId<C, CoordinateId>::add(newCoord, newId);
}

template<typename C>
void CoordinateGroup<C>::add(vector<C*>& newCoords, bool newId) {
    return GroupId<C, CoordinateId>::add(newCoords, newId);
}

template<typename C>
C* CoordinateGroup<C>::add(const CVecD3& newPosition,
                           const bool canOverlap) {
    vector<const C*> res;
    vector<CVecD3> aux;
    aux.push_back(newPosition);
    res = add(aux, canOverlap);
    if(!res.empty())
        return res[0];
    return NULL;
}

template<typename C>
vector<C*> CoordinateGroup<C>::add(const vector<CVecD3>& newPos,
                                   const bool canOverlap) {
    vector<C*> newCoords;
    for(uint i = 0; i < newPos.size(); i++) {
        if (get(newPos[i]) == NULL || canOverlap) {
            CoordD3* newCoord = new CoordD3(newPos[i]);
            if (newCoord->template is<C>()) {
                newCoords.push_back(newCoord->template castTo<C>());
            } else {
                delete newCoord;
            }
        }
    }
    add(newCoords, true);
    buildIndex(newCoords);
    return newCoords;
}

template<typename C>
void CoordinateGroup<C>::applyScalingFactor(const double factor) {
    for(unsigned i = 0; i < this->size(); i++) {
        if (this->element_[i]->template is<CoordD3>()) {
            CoordD3* ptr = this->element_[i]->template castTo<CoordD3>();
            *ptr *= factor;
        }
    }
}

template<typename C>
void CoordinateGroup<C>::printInfo() const {
    cout<< "--- CoordinateGroup info ---" << endl;
    for (unsigned i = 0; i < this->size(); i++) {
        this->element_[i]->printInfo();
        cout << endl;
    }
    cout<< "Total: " << this->size() << " coordinates." << endl;
}

template<typename C>
void CoordinateGroup<C>::buildIndex(const vector<C*>& coords) {
    for (unsigned i = 0; i < coords.size(); i++) {
        if (coords[i]->template is<CoordD3>())
            index_.insert(coords[i]->template castTo<CoordD3>());
    }
}
