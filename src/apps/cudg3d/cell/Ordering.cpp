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

#include "../../dgtd/core/Ordering.h"

size_t Ordering::globalSize = 0;
size_t Ordering::localOffset = 0;
size_t Ordering::localSize = 0;
ElemId Ordering::offsetId(0);
ElemId* Ordering::idOfRelPos = NULL;
size_t* Ordering::relPosOfId = NULL;

Ordering::Ordering() {
}

Ordering::~Ordering() {
}

ElemId Ordering::getIdOfGlobalRelPos(const size_t rp) const {
	assert(idOfRelPos != NULL);
	return idOfRelPos[rp];
}

size_t Ordering::getGlobalRelPosOfId(const ElemId id) const {
	assert(relPosOfId != NULL);
	return relPosOfId[id.tosize_t() - offsetId.tosize_t()];
}

ElemId Ordering::getIdOfRelPos(const size_t rp) const {
	assert(idOfRelPos != NULL);
	assert(idOfRelPos[rp + localOffset] >= offsetId);
	return idOfRelPos[rp + localOffset];
}

size_t Ordering::getRelPosOfId(const ElemId id) const {
	assert(relPosOfId != NULL);
	assert((relPosOfId[id.tosize_t() - offsetId.tosize_t()] - localOffset) < localSize);
	return relPosOfId[id.tosize_t() - offsetId.tosize_t()] - localOffset;
}

size_t Ordering::getGlobalSize() const {
	assert(globalSize != 0);
	return globalSize;
}

size_t Ordering::getLocalSize() const {
	assert(localSize != 0);
	return localSize;
}

bool Ordering::isLocalId(const ElemId id) const {
	assert(relPosOfId != NULL);
	assert(id.tosize_t() >= offsetId.tosize_t());
	size_t rp = relPosOfId[id.tosize_t() - offsetId.tosize_t()];
	if (localOffset > rp || localSize == rp - localOffset) {
		return false;
	}
	return ((rp - localOffset) < localSize);
}

bool Ordering::checkRelPosOfId() const {
	bool ok = true;
	for (size_t i = 0; i < globalSize; i++) {
		ElemId id = getIdOfGlobalRelPos(i);
		size_t rp = getGlobalRelPosOfId(id);
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

void Ordering::buildRelPosOfIds(const DynMatrix<size_t>& list) {
	// Changes the ordering according to list.
	// list is a matrix in which the first column contains ids.
	size_t nK = list.nRows();
	setGlobalSize(nK);
	DynMatrix<size_t> aux(nK,2);
	for (size_t i = 0; i < nK; i++) {
		aux(i,0) = i;
		aux(i,1) = list(i,0);
	}
	aux.sortRows_omp(1,1);
	offsetId = ElemId(aux(0,1));
	if (idOfRelPos != NULL) {
		delete [] idOfRelPos;
	}
	idOfRelPos = new ElemId[nK];
	for (size_t i = 0; i < nK; i++) {
		idOfRelPos[aux(i,0)] = ElemId(aux(i,1));
		// Checks that ids are consecutive.
		if (i > 0 && (aux(i-1,1)+1 != aux(i,1))) {
			throw Error("Ids in elements are non consecutive.");
		}
	}
	if (relPosOfId != NULL) {
		delete [] relPosOfId;
	}
	relPosOfId = new size_t[nK];
	for (size_t i = 0; i < nK; i++) {
		relPosOfId[aux(i,1) - offsetId.tosize_t()] = aux(i,0);
	}
	assert(checkRelPosOfId());
}

void Ordering::setGlobalSize(
 const size_t globalSize_) {
	globalSize = globalSize_;
}

void Ordering::setLocalSizeAndOffset(
 const size_t localSize_,
 const size_t localOffset_) {
	localSize = localSize_;
	localOffset = localOffset_;
}

void Ordering::printOrderingInfo() const {
	cout << "RelPosOfIds: " << relPosOfId << endl;
	for (size_t i = 0; i < globalSize; i++) {
	    ElemId id(i + offsetId.tosize_t());
		cout << getGlobalRelPosOfId(id) << " ";
	}
	cout << endl;
	cout << "IdsOfRelPos: " << idOfRelPos << endl;
	for (size_t i = 0; i < globalSize; i++) {
		cout << getIdOfGlobalRelPos(i) << " ";
	}
	cout << endl;
}

bool Ordering::checkLocalIds(
 const vector<vector<ElemId> >& partIds,
 const size_t task) {
	vector<ElemId> localId = partIds[task];
	size_t nK = localId.size();
	assert(nK == localSize);
	bool localPartIdsAreLocalIds = true;
	for (size_t i = 0; i < nK; i++) {
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
	for (size_t i = 0; i < nK; i++) {
		ElemId id = getIdOfRelPos(i);
		bool isPresent = false;
		for (size_t j = 0; j < nK; j++) {
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
