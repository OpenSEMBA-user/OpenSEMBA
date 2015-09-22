/*
 * GroupCoordinates.hpp
 *
 *  Created on: Aug 29, 2012
 *      Author: luis
 */

#include "GroupCoordinates.h"

template<typename C>
GroupCoordinates<C>::GroupCoordinates() {

}

template<typename C>
GroupCoordinates<C>::GroupCoordinates(const vector<CVecR3>& pos) {
    addPos(pos, true);
}

template<typename C>
GroupCoordinates<C>::GroupCoordinates(const vector<CVecI3>& pos) {
    addPos(pos, true);
}

template<typename C> template<typename C2>
GroupCoordinates<C>::GroupCoordinates(C2* elem)
:   GroupId<C, CoordinateId>(elem) {
    postprocess_(0);
}

template<typename C> template<typename C2>
GroupCoordinates<C>::GroupCoordinates(const std::vector<C2*>& elems)
:   GroupId<C, CoordinateId>(elems) {
    postprocess_(0);
}

template<typename C> template<typename C2>
GroupCoordinates<C>::GroupCoordinates(VectorPtr<C2>& rhs)
:   GroupId<C, CoordinateId>(rhs) {
    postprocess_(0);
}

template<typename C> template<typename C2>
GroupCoordinates<C>::GroupCoordinates(const VectorPtr<C2>& rhs)
:   GroupId<C, CoordinateId>(rhs) {
    postprocess_(0);
}

template<typename C>
GroupCoordinates<C>::GroupCoordinates(VectorPtr<C>& rhs)
:   GroupId<C, CoordinateId>(rhs) {
    postprocess_(0);
}

template<typename C> template<typename C2>
GroupCoordinates<C>::GroupCoordinates(VectorPtr<C2>&& rhs)
:   GroupId<C, CoordinateId>(std::move(rhs)) {
    postprocess_(0);
}

template<typename C>
GroupCoordinates<C>::GroupCoordinates(VectorPtr<C>&& rhs)
:   GroupId<C, CoordinateId>(std::move(rhs)) {
    postprocess_(0);
}

template<typename C>
GroupCoordinates<C>::~GroupCoordinates() {

}

template<typename C>
GroupCoordinates<C>& GroupCoordinates<C>::operator=(VectorPtr<C>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    GroupId<C, CoordinateId>::operator=(rhs);
    postprocess_(0);
    return *this;
}

template<typename C>
GroupCoordinates<C>& GroupCoordinates<C>::operator=(VectorPtr<C>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    GroupId<C, CoordinateId>::operator=(std::move(rhs));
    postprocess_(0);
    return *this;
}

template<typename C>
void GroupCoordinates<C>::clear() {
    GroupId<C,CoordinateId>::clear();
    indexUnstr_.clear();
    indexStr_.clear();
}

template<typename C>
const CoordR3* GroupCoordinates<C>::getPos(const CVecR3& position) const {
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
const CoordI3* GroupCoordinates<C>::getPos(const CVecI3& position) const {
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
VectorPtr<C> GroupCoordinates<C>::add(VectorPtr<C>& rhs) {
    UInt lastSize = this->size();
    GroupId<C, CoordinateId>::add(rhs);
    postprocess_(lastSize);
    return rhs;
}

template<typename C>
VectorPtr<C> GroupCoordinates<C>::add(VectorPtr<C>&& rhs) {
    UInt lastSize = this->size();
    GroupId<C, CoordinateId>::add(std::move(rhs));
    postprocess_(lastSize);
    return rhs;
}

template<typename C>
const C* GroupCoordinates<C>::addPos(const CVecR3& newPosition,
                                     const bool canOverlap) {
    vector<CVecR3> aux;
    aux.push_back(newPosition);
    GroupCoordinates<C> res = addPos(aux, canOverlap);
    if (res.empty()) {
        return getPos(newPosition);
    }
    return res(0);
}

template<typename C>
GroupCoordinates<C> GroupCoordinates<C>::addPos(const vector<CVecR3>& newPos,
                                                const bool canOverlap) {
    vector<C*> newCoords;
    newCoords.reserve(newPos.size());
    for(UInt i = 0; i < newPos.size(); i++) {
        if (canOverlap || (getPos(newPos[i]) == NULL)) {
            CoordR3* newCoord = new CoordR3(newPos[i]);
            if (newCoord->template is<C>()) {
                newCoords.push_back(newCoord->castTo<C>());
            } else {
                delete newCoord;
            }
        }
    }
    return this->addId(newCoords);
}

template<typename C>
const C* GroupCoordinates<C>::addPos(const CVecI3& newPosition,
                                     const bool canOverlap) {
    vector<CVecI3> aux;
    aux.push_back(newPosition);
    GroupCoordinates<C> res = addPos(aux, canOverlap);
    if (res.empty()) {
        return getPos(newPosition);
    }
    return res(0);
}

template<typename C>
GroupCoordinates<C> GroupCoordinates<C>::addPos(const vector<CVecI3>& newPos,
                                              const bool canOverlap) {
    vector<C*> newCoords;
    for(UInt i = 0; i < newPos.size(); i++) {
        if (canOverlap || (getPos(newPos[i]) == NULL)) {
            CoordI3* newCoord = new CoordI3(newPos[i]);
            if (newCoord->template is<C>()) {
                newCoords.push_back(newCoord->template castTo<C>());
            } else {
                delete newCoord;
            }
        }
    }
    return this->addId(newCoords);
}

template<typename C>
void GroupCoordinates<C>::remove(const UInt& pos) {
    if (this->get(pos)->template is<CoordR3>()) {
        indexUnstr_.erase(this->get(pos)->template castTo<CoordR3>());
    }
    if (this->get(pos)->template is<CoordI3>()) {
        indexStr_.erase(this->get(pos)->template castTo<CoordI3>());
    }
    GroupId<C, CoordinateId>::remove(pos);
}

template<typename C>
void GroupCoordinates<C>::remove(const std::vector<UInt>& pos) {
    for (UInt i = 0; i < pos.size(); i++) {
        if (this->get(pos[i])->template is<CoordR3>()) {
            indexUnstr_.erase(this->get(pos[i])->template castTo<CoordR3>());
        }
        if (this->get(pos[i])->template is<CoordI3>()) {
            indexStr_.erase(this->get(pos[i])->template castTo<CoordI3>());
        }
    }
    GroupId<C, CoordinateId>::remove(pos);
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
void GroupCoordinates<C>::postprocess_(const UInt fistStep) {
    for (UInt i = fistStep; i < this->size(); i++) {
        if (this->get(i)->template is<CoordR3>()) {
            indexUnstr_.insert(this->get(i)->template castTo<CoordR3>());
        }
        if (this->get(i)->template is<CoordI3>()) {
            indexStr_.insert(this->get(i)->template castTo<CoordI3>());
        }
    }
}
