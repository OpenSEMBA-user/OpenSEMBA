/*
 * CoordinateGroup.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: luis
 */

#include "CoordinateGroup.h"

CoordinateGroup::CoordinateGroup() {
}

CoordinateGroup::~CoordinateGroup() {
}

CoordinateGroup::CoordinateGroup(
        const vector<CoordD3>& coord) {
    for (uint i = 0; i < coord.size(); i++) {
        pair<uint,CoordD3*> aux(coord[i].getId(), new CoordD3(coord[i]));
        coord_.insert(aux);
        index_.insert(aux.second);
    }
}

CoordinateGroup::CoordinateGroup(
        const vector<CVecD3>& pos) {
    uint id = 0;
    for (uint i = 0; i < pos.size(); i++) {
        ++id;
        pair<uint,CoordD3*> aux(id, new CoordD3(id, pos[i]));
        coord_.insert(aux);
        index_.insert(aux.second);
    }
}

const CoordD3*
CoordinateGroup::getPtrToId(const unsigned int id) const {
    return (*this)(id);
}

void
CoordinateGroup::applyScalingFactor(const double factor) {
    map<uint,CoordD3*>::iterator it;
    for (it=coord_.begin(); it != coord_.end(); ++it) {
        *(it->second) *= factor;
    }
}

void
CoordinateGroup::add(const vector<CVecD3>& newPos, const bool canOverlap) {
    uint newId;
    for (uint i = 0; i < newPos.size(); i++) {
        if (get(newPos[i]) == NULL || canOverlap) {
            if (!coord_.empty()) {
                newId = coord_.rbegin()->first + 1;
            } else {
                newId = 1;
            }
            pair<uint,CoordD3*> aux(newId, new CoordD3(newId, newPos[i]));
            coord_.insert(aux);
            index_.insert(aux.second);
        }
    }
}

void
CoordinateGroup::add(const CVecD3& newPosition) {
    vector<CVecD3> aux;
    aux.push_back(newPosition);
    add(aux);
}

const CoordD3*
CoordinateGroup::operator () (const uint id) const {
    assert(coord_.find(id) != coord_.end());
    return coord_.find(id)->second;
}

void
CoordinateGroup::printInfo() const {
    cout<< "--- CoordinateGroup info ---" << endl;
    map<uint,CoordD3*>::const_iterator it;
    for (it=coord_.begin(); it != coord_.end(); ++it) {
        it->second->printInfo();
        cout << endl;
    }
    cout<< "Total: " << size() << " coordinates." << endl;
}

const Coordinate<double, 3>*
CoordinateGroup::get(const CVecD3& position) const {
    multiset<CoordD3*, lexCompareCoord>::iterator it;
    CoordD3 aux(position);
    it = index_.find(&aux);
    if (it != index_.end()) {
        const CoordD3* res = *it;
        return (*this)(res->getId());
    } else {
        return NULL;
    }
}

CoordinateGroup&
CoordinateGroup::operator =(const CoordinateGroup& rhs) {
    if (this == &rhs) {
        return *this;
    }
    map<uint,CoordD3*>::const_iterator it;
    for (it=rhs.coord_.begin(); it != rhs.coord_.end(); ++it) {
        pair<uint,CoordD3*> aux(it->first, new CoordD3(*(it->second)));
        coord_.insert(aux);
        index_.insert(aux.second);
    }
    return *this;
}

vector<const CoordD3*>
CoordinateGroup::getAll() const {
    vector<const CoordD3*> res;
    res.reserve(size());
    map<uint,CoordD3*>::const_iterator it;
    for (it=coord_.begin(); it != coord_.end(); ++it) {
        res.push_back(it->second);
    }
    return res;
}
