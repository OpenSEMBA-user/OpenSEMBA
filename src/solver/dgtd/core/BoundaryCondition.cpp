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

#include "BoundaryCondition.h"

BoundaryCondition::BoundaryCondition() {
	cell = NULL;
	face = 0;
	condition = NULL;
}

BoundaryCondition::~BoundaryCondition() {

}

const CellTet<ORDER_N>*
BoundaryCondition::getCellD() const {
	assert(false);
	return NULL;
}

uint
BoundaryCondition::getFaceD() const {
	assert(false);
	return 0;
}

void
BoundaryCondition::printInfo() const {
	cout << "--- BC info ---" << endl;
	cout << "Cell Id:" << cell->getId() << " Face: " << face << endl;
	cout << "Condition: " << endl;
	condition->printInfo();
}

EMSourceBC::EMSourceBC() {

}

EMSourceBC::~EMSourceBC() {

}

EMSourceBC::EMSourceBC(const BoundaryCondition& param) {
	cell = param.cell;
	face = param.face;
	condition = param.condition;
	//
	check();
}
 
EMSourceBC::EMSourceBC(
 const CellTet<ORDER_N>* e,
 const uint f,
 const Condition* bc) {
	cell = e;
	face = f;
	condition = bc;
	check();
}
 
EMSourceBC&
EMSourceBC::operator=(const EMSourceBC& rhs) {
	if (this == &rhs)
		return *this;
	cell = rhs.cell;
	face = rhs.face;
	condition = rhs.condition;
	return *this;
}
//
void
EMSourceBC::check() const {
	if (!condition->isEMSource()) {
		cerr << "ERROR@EMSourceBC ctor" << endl;
		exit(BC_ERROR);
	}
}
// =============== PhysicalModelBC ============================================
PhysicalModelBC::PhysicalModelBC() {

}

PhysicalModelBC::~PhysicalModelBC() {

}

PhysicalModelBC::PhysicalModelBC(const BoundaryCondition& param) {
	cell = param.cell;
	face = param.face;
	condition = param.condition;
	//
	checkIsPMValidForSurfaces();
}
 
PhysicalModelBC::PhysicalModelBC(
 const CellTet<ORDER_N>* cell_,
 uint face_,
 const Condition* bc) {
	cell = cell_;
	face = face_;
	condition = bc;
	if (!checkIsPMValidForSurfaces()) {
		cout << "Invalid PM on Surface." << endl;
		cout << "Cell:" << cell->getId() << " Face: " << face << endl;
		cout << "Physical Model: " << endl;
		condition->printInfo();
		exit(-1);
	}
}

PhysicalModelBC&
PhysicalModelBC::operator=(const PhysicalModelBC& rhs) {
	if (this == &rhs) {
		return *this;
	}
	cell = rhs.cell;
	face = rhs.face;
	condition = rhs.condition;
	return *this;
}
//
bool
PhysicalModelBC::checkIsPMValidForSurfaces() const {
	if (!condition->isPMPredefined() && !condition->isSurface()) {
		condition->printInfo();
		cerr << "ERROR@PhysicalModelBC ctor" << endl;
		return false;
	}
	return true;
}

SurfaceImpedanceBC::SurfaceImpedanceBC() {
	cellD = NULL;
	faceD = 0;
}

SurfaceImpedanceBC::~SurfaceImpedanceBC() {

}

SurfaceImpedanceBC::SurfaceImpedanceBC(
 const CellTet<ORDER_N>* cell_,
 const uint face_,
 const CellTet<ORDER_N>* cellD_,
 const uint faceD_,
 const Condition* cond_) {
	cell = cell_;
	face = face_;
	cellD = cellD_;
	faceD = faceD_;
	condition = cond_;
	assert(cell != NULL);
	assert(cellD != NULL);
	assert(condition != NULL);
}

SurfaceImpedanceBC&
SurfaceImpedanceBC::operator=(const SurfaceImpedanceBC &rhs) {
	if (this == &rhs) {
		return *this;
	}
	cell = rhs.cell;
	face = rhs.face;
	cellD = rhs.cellD;
	faceD = rhs.faceD;
	condition = rhs.condition;
	return *this;
}

const CellTet<ORDER_N>*
SurfaceImpedanceBC::getCellD() const {
	return cellD;
}

uint
SurfaceImpedanceBC::getFaceD() const {
	return faceD;
}

