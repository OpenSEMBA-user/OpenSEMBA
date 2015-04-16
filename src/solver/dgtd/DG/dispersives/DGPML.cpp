/*
 * SolverPML.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: luis
 */

#include "DGPML.h"

DGPML::DGPML() {
	nElem = 0;
	dof = 0;
	elem  = NULL;
	sig = sigDefault;
	useConstantConductivity = false;
	sig1 = NULL;
	sig2 = NULL;
	sig3 = NULL;
	sig12 = NULL;
	sig23 = NULL;
	sig31 = NULL;
	sig11 = NULL;
	sig22 = NULL;
	sig33 = NULL;
}

DGPML::~DGPML() {

}

void
DGPML::initElemsAndDOFs(
 const PMVolumePML& mat,
 const CellGroup& cells) {
	// Initializes Element list and dof.
	vector<uint> auxList;
	auxList.reserve(cells.getLocalSize());
	for (uint e = 0; e < cells.getLocalSize(); e++) {
		const CellTet<ORDER_N>* cell = cells(e);
		uint id = cell->getId();
		if (cell->material->getId() == mat.getId()) {
			auxList.push_back(cells.getRelPosOfId(id));
		}
		nElem = auxList.size();
		elem = new uint[nElem];
		for (uint j = 0; j < nElem; j++) {
			elem[j] = auxList[j];
		}
	}
	dof = np * nElem;
}

void
DGPML::initConductivity(
 double **sigma,
 const uint orientation,
 const PMVolumePML& mat,
 const CellGroup& cells) {
	StaMatrix<double,np,np> auxSig;
	for (uint e = 0; e < nElem; e++) {
		uint id = cells.getIdOfRelPos(elem[e]);
		const CellTet<ORDER_N>* cell;
		cell = cells.getPtrToCellWithId(id);
		auxSig =
		 cell->getConductivityWithGeometricProfile(mat, orientation, sig);
		auxSig.convertToArray(MATRICES_ROW_MAJOR, sigma[e]);
	}
}

void
DGPML::initConductivityMatrices(
 const PMVolumePML& mat,
 const CellGroup& cells) {
	assert(elem != NULL);
	if (mat.isUniaxial()) {
		sig1 = new double*[nElem];
		sig11 = new double*[nElem];
		for (uint e = 0; e < nElem; e++) {
			sig1[e] = new double[np*np];
			sig11[e] = new double[np*np];
		}
		initConductivity(sig1, 1, mat, cells);
		initConductivity(sig11, 11, mat, cells);
	} else if (mat.isBiaxial()) {
		sig1 = new double*[nElem];
		sig2 = new double*[nElem];
		sig11 = new double*[nElem];
		sig22 = new double*[nElem];
		sig12= new double*[nElem];
		for (uint e = 0; e < nElem; e++) {
			sig1[e] = new double[np*np];
			sig2[e] = new double[np*np];
			sig11[e] = new double[np*np];
			sig22[e] = new double[np*np];
			sig12[e] = new double[np*np];
		}
		initConductivity(sig1, 1, mat, cells);
		initConductivity(sig2, 2, mat, cells);
		initConductivity(sig11, 11, mat, cells);
		initConductivity(sig22, 22, mat, cells);
		initConductivity(sig12, 12, mat, cells);
	} else {
		assert(mat.isTriaxial());
		sig1 = new double*[nElem];
		sig2 = new double*[nElem];
		sig3 = new double*[nElem];
		sig11 = new double*[nElem];
		sig22 = new double*[nElem];
		sig33 = new double*[nElem];
		sig12= new double*[nElem];
		sig23= new double*[nElem];
		sig31= new double*[nElem];
		for (uint e = 0; e < nElem; e++) {
			sig1[e] = new double[np*np];
			sig2[e] = new double[np*np];
			sig3[e] = new double[np*np];
			sig11[e] = new double[np*np];
			sig22[e] = new double[np*np];
			sig33[e] = new double[np*np];
			sig12[e] = new double[np*np];
			sig23[e] = new double[np*np];
			sig31[e] = new double[np*np];
		}
		initConductivity(sig1, 1, mat, cells);
		initConductivity(sig2, 2, mat, cells);
		initConductivity(sig3, 3, mat, cells);
		initConductivity(sig11, 11, mat, cells);
		initConductivity(sig22, 22, mat, cells);
		initConductivity(sig33, 33, mat, cells);
		initConductivity(sig12, 12, mat, cells);
		initConductivity(sig23, 23, mat, cells);
		initConductivity(sig31, 31, mat, cells);
	}
}

void
DGPML::init(
 const PMVolumePML& mat,
 const CellGroup& cells) {
	// Initializes Element list and dof.
	initElemsAndDOFs(mat, cells);
	// Initializes conductivity matrices.
	if (!useConstantConductivity) {
		initConductivityMatrices(mat, cells);
	}
}

void DGPML::addJumps(Field<double, 3>& dE, Field<double, 3>& dH,
		Field<double, 3>& E, Field<double, 3>& H, const uint e1,
		const uint e2) {
}
