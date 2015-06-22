/*
 * SolverLM.cpp
 *
 *  Created on: Sep 3, 2014
 *      Author: luis
 */

#include "DGLowMemory.h"

DGLowMemory::DGLowMemory(
 const SmbData* mesh, const CellGroup* cells,
 const BCGroup* bc, Comm* comm, const ArgumentsCudg3d* arg) {
}

DGLowMemory::~DGLowMemory() {
	// TODO Auto-generated destructor stub
}

void
DGLowMemory:: printInfo() const {
	cout<< " --- SolverLM info ---" << endl;
	cout<< "PRINT INFO NOT IMPLEMENTED" << endl;
}

void
DGLowMemory::updateFieldsWithRes(
 const UInt e1, const UInt e2,	const Real rkb) {
	updateFieldsWithResBase(e1,e2,rkb);
}

void
DGLowMemory::computeCurlsInRHSElectric(const UInt e1, const UInt e2) {

}

void
DGLowMemory::computeCurlsInRHSMagnetic(const UInt e1, const UInt e2) {
}

void
DGLowMemory::addFluxesToRHSElectric(const UInt e1, const UInt e2) {
}

void
DGLowMemory::addFluxesToRHSMagnetic(const UInt e1, const UInt e2) {
}

void
DGLowMemory::addRHSToResidueElectric(
 const UInt e1, const UInt e2,	const Real rkdt) {
}

void
DGLowMemory::addRHSToResidueMagnetic(
 const UInt e1, const UInt e2,	const Real rkdt) {
}

void
DGLowMemory::addRHSToFieldsElectric(
 const UInt e1, const UInt e2,	const Real rkdt) {
}

void
DGLowMemory::addRHSToFieldsMagnetic(const UInt e1, const UInt e2,
		const Real rkdt) {
}

void
DGLowMemory::computeJumps(const UInt e1, const UInt e2,
		const Real localTime, const Real minDT) {
}

void
DGLowMemory::computePolarizationCurrentsRHS(const UInt e1, const UInt e2) {
}

void
DGLowMemory::computePolarizationCurrentsRHSElectric(const UInt e1,
		const UInt e2) {
}

void
DGLowMemory::computePolarizationCurrentsRHSMagnetic(const UInt e1,
		const UInt e2) {
}

void
DGLowMemory::LTSSaveFieldsAndResidues(const UInt fKSave, const UInt lKSave) {
}

void
DGLowMemory::LTSLoadFieldsAndResidues(const UInt fKSave, const UInt lKSave) {
}

void
DGLowMemory::addRHSToRes(const UInt e1, const UInt e2, const Real rka,
		const Real dt) {
}

void
DGLowMemory::buildMaterials(const CellGroup& cells,
		const ArgumentsCudg3d* arg) {
}

void
DGLowMemory::buildCMatrices(const CellGroup& cell) {
}

void
DGLowMemory::assignMatrices(const CellGroup& cells) {
}
