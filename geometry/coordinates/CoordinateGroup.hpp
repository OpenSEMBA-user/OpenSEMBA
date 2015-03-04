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
:   GroupWithIdBase<C, CoordinateId>(coord) {
    for (uint i = 0; i < this->size(); i++) {
        if (this->element_[i]->template isOf<CoordD3>())
            index_.insert(this->element_[i]->template castTo<CoordD3>());
    }
}

template<typename C>
CoordinateGroup<C>::CoordinateGroup(const vector<CVecD3>& pos) {
    add(pos);
}

template<typename C>
CoordinateGroup<C>::CoordinateGroup(const CoordinateGroup<C>& rhs)
:   GroupWithIdBase<C, CoordinateId>(rhs),
    index_(rhs.index_) {
    
}

template<typename C>
CoordinateGroup<C>::~CoordinateGroup() {

}

template<typename C>
CoordinateGroup<C>& CoordinateGroup<C>::operator=(
    const CoordinateGroup<C>& rhs) {
    
    if (this == &rhs) {
        return *this;
    }
    
    GroupWithIdBase<C, CoordinateId>::operator=(rhs);
    for (uint i = 0; i < this->size(); i++) {
        if (this->element_[i]->template isOf<CoordD3>())
            index_.insert(this->element_[i]->template castTo<CoordD3>());
    }
    
    return *this;
}

template<typename C>
vector<const CoordD3*> CoordinateGroup<C>::getAll() const {
    vector<const CoordD3*> res;
    res.reserve(this->size());
    for (unsigned i = 0; i < this->size(); i++) {
        if (this->element_[i]->template isOf<CoordD3>())
            res.push_back(this->element_[i]->template castTo<CoordD3>());
    }
    return res;
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
CoordinateId CoordinateGroup<C>::add(const CVecD3& newPosition) {
    vector<CoordinateId> ids;
    vector<CVecD3> aux;
    aux.push_back(newPosition);
    ids = add(aux);
    if(!ids.empty())
        return ids[0];
    else
        return 0;
}

template<typename C>
vector<CoordinateId> CoordinateGroup<C>::add(const vector<CVecD3>& newPos,
                             const bool canOverlap) {
    CoordinateId newId;
    for (uint i = 0; i < newPos.size(); i++) {
        if (get(newPos[i]) == NULL || canOverlap) {
            CoordD3* newCoord = new CoordD3(newPos[i]);
            C* newCoordC = newCoord->template castTo<C>();
            if(newCoordC == NULL) {
                delete newCoord;
                continue;
            }
            
            newId = GroupWithIdBase<C, CoordinateId>::add(newCoordC);
            delete newCoord;
            const CoordD3* inserted =
                this->getPtrToId(newId)->template castTo<CoordD3>();
            index_.insert(inserted);
        }
    }
}

template<typename C>
void CoordinateGroup<C>::applyScalingFactor(const double factor) {
    for(unsigned i = 0; i < this->size(); i++) {
        if (this->element_[i]->template isOf<CoordD3>()) {
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
