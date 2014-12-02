/*
 * CoordinateGroup.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: luis
 */

#include "CoordinateGroup.h"
// =============== CoordinateGroup ============================================
CoordinateGroup::CoordinateGroup() {
	offsetId = 0;
}

CoordinateGroup::~CoordinateGroup() {

}

CoordinateGroup::CoordinateGroup(
 const vector<Coordinate<double,3> >& coord_) {
	coord.resize(coord_.size());
	for (uint i = 0; i < coord_.size(); i++) {
		coord[i] = new Coordinate<double,3>(coord_[i]);
	}
	offsetId = coord[0]->id;
	//
//	for (uint i = 0; i < coord.size(); i++) {
//		index.insert(coord[i]);
//	}
	//
	check();
}

const Coordinate<double,3>*
CoordinateGroup::getPtrToId(const unsigned int id) const {
	return coord[id - offsetId];
}

void
CoordinateGroup::applyScalingFactor(const double factor) {
	for (unsigned int i = 0; i < size(); i++) {
		*coord[i] *= factor;
	}
}

void
CoordinateGroup::add(const vector<CVecD3>& newPos) {
	checkIdsAreConsecutive();
	uint lastId = coord.back()->getId();
	coord.reserve(coord.size() + newPos.size());
	for (uint i = 0; i < newPos.size(); i++) {
		coord.push_back(new Coordinate<double,3>(++lastId, newPos[i]));
	}
}

//void
//CoordinateGroup::add(const CVecD3& newPosition) {
//	vector<CVecD3> aux;
//	aux.push_back(newPosition);
//	add(aux);
//}

void
CoordinateGroup::check() const {
	checkIdsAreConsecutive();
}

void
CoordinateGroup::printInfo() const {
	cout<< "--- CoordinateGroup info ---" << endl;
	for (unsigned int i = 0; i < size(); i++) {
		coord[i]->printInfo();
		cout << endl;
	}
	cout<< "Total: " << size() << " coordinates." << endl;
}

const Coordinate<double, 3>*
CoordinateGroup::get(const CVecD3& position) const {
	multiset<CoordD3*, lexCompareCoord>::iterator it;
	Coordinate<double,3> aux(position);
	it = index.find(&aux);
	if (it != index.end()) {
		const CoordD3* res = *it;
		return getPtrToId(res->getId());
	} else {
		return NULL;
	}
}

void
CoordinateGroup::checkIdsAreConsecutive() const {
	unsigned int currentId = offsetId;
	for (unsigned int i = 1; i < size(); i++) {
		if (coord[i]->id == currentId + 1) {
			currentId++;
		} else {
			cerr<< "ERROR @ CoordinateGroup: " << endl
				<< "Ids are not consecutive" << endl;
		}
	}
}

CoordinateGroup&
CoordinateGroup::operator =(const CoordinateGroup& rhs) {
	if (this == &rhs) {
		return *this;
	}
	offsetId = rhs.offsetId;
	coord.resize(rhs.coord.size());
	for (uint i = 0; i < coord.size(); i++) {
		coord[i] = new Coordinate<double,3> (*rhs.coord[i]);
	}
	return *this;
}
