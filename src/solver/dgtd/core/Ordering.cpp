// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
/*
 * Ordering.cpp
 *
 *  Created on: Jun 12, 2013
 *      Author: luis
 */

#include "Ordering.h"

UInt Ordering::globalSize = 0;
UInt Ordering::localOffset = 0;
UInt Ordering::localSize = 0;
UInt Ordering::offsetId = 0;
ElementId* Ordering::idOfRelPos = NULL;
UInt* Ordering::relPosOfId = NULL;

Ordering::Ordering() {
}

Ordering::~Ordering() {
	// TODO Auto-generated destructor stub
}

ElementId Ordering::getIdOfGlobalRelPos(const UInt rp) const {
	assert(idOfRelPos != NULL);
	return idOfRelPos[rp];
}

UInt Ordering::getGlobalRelPosOfId(const ElementId id) const {
	assert(relPosOfId != NULL);
	return relPosOfId[id.toUInt() - offsetId];
}

ElementId Ordering::getIdOfRelPos(const UInt rp) const {
	assert(idOfRelPos != NULL);
	assert(idOfRelPos[rp + localOffset] >= offsetId);
	return idOfRelPos[rp + localOffset];
}

UInt Ordering::getRelPosOfId(const ElementId id) const {
	assert(relPosOfId != NULL);
	assert((relPosOfId[id.toUInt() - offsetId] - localOffset) < localSize);
	return relPosOfId[id.toUInt() - offsetId] - localOffset;
}

UInt Ordering::getGlobalSize() const {
	assert(globalSize != 0);
	return globalSize;
}

UInt Ordering::getLocalSize() const {
	assert(localSize != 0);
	return localSize;
}

bool Ordering::isLocalId(const UInt id) const {
	assert(relPosOfId != NULL);
	assert(id >= offsetId);
	UInt rp = relPosOfId[id - offsetId];
	if (localOffset > rp || localSize == rp - localOffset) {
		return false;
	}
	return ((rp - localOffset) < localSize);
}

bool Ordering::checkRelPosOfId() const {
	bool ok = true;
	for (UInt i = 0; i < globalSize; i++) {
		ElementId id = getIdOfGlobalRelPos(i);
		UInt rp = getGlobalRelPosOfId(id);
		if (i != rp) {
			cout << "id:" << id << " has rp " << rp
			 << " and should be " << i << endl;
			ok = false;
		}
	}
	if (!ok) {
		throw Error("Check above ids.");
	}
	return ok;
}

void Ordering::buildRelPosOfIds(const DynMatrix<UInt>& list) {
	// Changes the ordering according to list.
	// list is a matrix in which the first column contains ids.
	UInt nK = list.nRows();
	setGlobalSize(nK);
	DynMatrix<UInt> aux(nK,2);
	for (UInt i = 0; i < nK; i++) {
		aux(i,0) = i;
		aux(i,1) = list(i,0);
	}
	aux.sortRows_omp(1,1);
	offsetId = aux(0,1);
	if (idOfRelPos != NULL) {
		delete [] idOfRelPos;
	}
	idOfRelPos = new ElementId[nK];
	for (UInt i = 0; i < nK; i++) {
		idOfRelPos[aux(i,0)] = ElementId(aux(i,1));
		// Checks that ids are consecutive.
		if (i > 0 && (aux(i-1,1)+1 != aux(i,1))) {
			throw Error("Ids in elements are non consecutive.");
		}
	}
	if (relPosOfId != NULL) {
		delete [] relPosOfId;
	}
	relPosOfId = new UInt[nK];
	for (UInt i = 0; i < nK; i++) {
		relPosOfId[aux(i,1) - offsetId] = aux(i,0);
	}
	assert(checkRelPosOfId());
}

void Ordering::setGlobalSize(
 const UInt globalSize_) {
	globalSize = globalSize_;
}

void Ordering::setLocalSizeAndOffset(
 const UInt localSize_,
 const UInt localOffset_) {
	localSize = localSize_;
	localOffset = localOffset_;
}

void Ordering::printOrderingInfo() const {
	cout << "RelPosOfIds: " << relPosOfId << endl;
	for (UInt i = 0; i < globalSize; i++) {
	    ElementId id(i + offsetId);
		cout << getGlobalRelPosOfId(id) << " ";
	}
	cout << endl;
	cout << "IdsOfRelPos: " << idOfRelPos << endl;
	for (UInt i = 0; i < globalSize; i++) {
		cout << getIdOfGlobalRelPos(i) << " ";
	}
	cout << endl;
}

bool Ordering::checkLocalIds(
 const vector<vector<UInt> >& partIds,
 const UInt task) {
	vector<ElementId> localId = partIds[task];
	UInt nK = localId.size();
	assert(nK == localSize);
	bool localPartIdsAreLocalIds = true;
	for (UInt i = 0; i < nK; i++) {
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
	for (UInt i = 0; i < nK; i++) {
		ElementId id = getIdOfRelPos(i);
		bool isPresent = false;
		for (UInt j = 0; j < nK; j++) {
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
