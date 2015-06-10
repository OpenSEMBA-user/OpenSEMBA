/*
 * Ordering.cpp
 *
 *  Created on: Jun 12, 2013
 *      Author: luis
 */

#include "Ordering.h"

uint Ordering::globalSize = 0;
uint Ordering::localOffset = 0;
uint Ordering::localSize = 0;
uint Ordering::offsetId = 0;
uint* Ordering::idOfRelPos = NULL;
uint* Ordering::relPosOfId = NULL;

Ordering::Ordering() {
}

Ordering::~Ordering() {
	// TODO Auto-generated destructor stub
}

uint
Ordering::getIdOfGlobalRelPos(const uint rp) const {
	assert(idOfRelPos != NULL);
	return idOfRelPos[rp];
}

uint
Ordering::getGlobalRelPosOfId(const uint id) const {
	assert(relPosOfId != NULL);
	return relPosOfId[id - offsetId];
}

uint
Ordering::getIdOfRelPos(const uint rp) const {
	assert(idOfRelPos != NULL);
	assert(idOfRelPos[rp + localOffset] >= offsetId);
	return idOfRelPos[rp + localOffset];
}

uint
Ordering::getRelPosOfId(const uint id) const {
	assert(relPosOfId != NULL);
	assert(
	 (relPosOfId[id - offsetId] - localOffset) < localSize);
	return relPosOfId[id - offsetId] - localOffset;
}

uint
Ordering::getGlobalSize() const {
	assert(globalSize != 0);
	return globalSize;
}

uint
Ordering::getLocalSize() const {
	assert(localSize != 0);
	return localSize;
}


bool
Ordering::isLocalId(const uint id) const {
	assert(relPosOfId != NULL);
	assert(id >= offsetId);
	uint rp = relPosOfId[id - offsetId];
	if (localOffset > rp || localSize == rp - localOffset) {
		return false;
	}
	return ((rp - localOffset) < localSize);
}

bool
Ordering::checkRelPosOfId() const {
	bool ok = true;
	for (uint i = 0; i < globalSize; i++) {
		uint id = getIdOfGlobalRelPos(i);
		uint rp = getGlobalRelPosOfId(id);
		if (i != rp) {
			cout << "id:" << id << " has rp " << rp
			 << " and should be " << i << endl;
			ok = false;
		}
	}
	if (!ok) {
		cout << "ERROR@checkRelPosOfId"<< endl;
		cout << "Check above ids." << endl;
	}
	return ok;
}

void
Ordering::buildRelPosOfIds(
 const DynMatrix<uint>& list) {
	// Changes the ordering according to list.
	// list is a matrix in which the first column contains ids.
	uint nK = list.nRows();
	setGlobalSize(nK);
	DynMatrix<uint> aux(nK,2);
	for (uint i = 0; i < nK; i++) {
		aux(i,0) = i;
		aux(i,1) = list(i,0);
	}
	aux.sortRows_omp(1,1);
	offsetId = aux(0,1);
	if (idOfRelPos != NULL) {
		delete [] idOfRelPos;
	}
	idOfRelPos = new uint[nK];
	for (uint i = 0; i < nK; i++) {
		idOfRelPos[aux(i,0)] = aux(i,1);
		// Checks that ids are consecutive.
		if (i > 0 && (aux(i-1,1)+1 != aux(i,1))) {
			cerr << endl << "ERROR@LSERKInfo ctor" << endl;
			cerr << endl << "Ids in elements are non consecutive." << endl;
		}
	}
	if (relPosOfId != NULL) {
		delete [] relPosOfId;
	}
	relPosOfId = new uint[nK];
	for (uint i = 0; i < nK; i++) {
		relPosOfId[aux(i,1) - offsetId] = aux(i,0);
	}
	assert(checkRelPosOfId());
}

void
Ordering::setGlobalSize(
 const uint globalSize_) {
	globalSize = globalSize_;
}

void
Ordering::setLocalSizeAndOffset(
 const uint localSize_,
 const uint localOffset_) {
	localSize = localSize_;
	localOffset = localOffset_;
}

void
Ordering::printOrderingInfo() const {
	cout << "RelPosOfIds: " << relPosOfId << endl;
	for (uint i = 0; i < globalSize; i++) {
		cout << getGlobalRelPosOfId(i + offsetId) << " ";
	}
	cout << endl;
	cout << "IdsOfRelPos: " << idOfRelPos << endl;
	for (uint i = 0; i < globalSize; i++) {
		cout << getIdOfGlobalRelPos(i) << " ";
	}
	cout << endl;
}

bool
Ordering::checkLocalIds(
 const vector<vector<uint> >& partIds,
 const uint task) {
	vector<uint> localId = partIds[task];
	uint nK = localId.size();
	assert(nK == localSize);
	bool localPartIdsAreLocalIds = true;
	for (uint i = 0; i < nK; i++) {
		if (!isLocalId(localId[i])) {
			cout << "Id: " << localId[i] << endl;
			localPartIdsAreLocalIds = false;
		}
	}
	if (!localPartIdsAreLocalIds) {
		cerr << endl << "ERROR@checkLocalIds()" << endl;
		cerr << endl << "Above ids are not local in cells ordering." << endl;
	}
	bool localIdsAreInLocalPartIds = true;
	for (uint i = 0; i < nK; i++) {
		uint id = getIdOfRelPos(i);
		bool isPresent = false;
		for (uint j = 0; j < nK; j++) {
			if (id == localId[j]) {
				isPresent = true;
				break;
			}
		}
		if (!isPresent) {
			cout << "El. in RelPos: " << i << "w. Id: " << id
			 << "is not in local id vector." << endl;
		}
	}
	if (!localIdsAreInLocalPartIds) {
		cerr << endl << "ERROR@checkLocalIds()" << endl;
		cerr << endl << "Inconsistent local ids." << endl;
	}
	return localPartIdsAreLocalIds && localIdsAreInLocalPartIds;
}

