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
 const uint e1, const uint e2,	const double rkb) {
	updateFieldsWithResBase(e1,e2,rkb);
}

void
DGLowMemory::computeCurlsInRHSElectric(const uint e1, const uint e2) {

}

void
DGLowMemory::computeCurlsInRHSMagnetic(const uint e1, const uint e2) {
}

void
DGLowMemory::addFluxesToRHSElectric(const uint e1, const uint e2) {
}

void
DGLowMemory::addFluxesToRHSMagnetic(const uint e1, const uint e2) {
}

void
DGLowMemory::addRHSToResidueElectric(
 const uint e1, const uint e2,	const double rkdt) {
}

void
DGLowMemory::addRHSToResidueMagnetic(
 const uint e1, const uint e2,	const double rkdt) {
}

void
DGLowMemory::addRHSToFieldsElectric(
 const uint e1, const uint e2,	const double rkdt) {
}

void
DGLowMemory::addRHSToFieldsMagnetic(const uint e1, const uint e2,
		const double rkdt) {
}

void
DGLowMemory::computeJumps(const uint e1, const uint e2,
		const double localTime, const double minDT) {
}

void
DGLowMemory::computePolarizationCurrentsRHS(const uint e1, const uint e2) {
}

void
DGLowMemory::computePolarizationCurrentsRHSElectric(const uint e1,
		const uint e2) {
}

void
DGLowMemory::computePolarizationCurrentsRHSMagnetic(const uint e1,
		const uint e2) {
}

void
DGLowMemory::LTSSaveFieldsAndResidues(const uint fKSave, const uint lKSave) {
}

void
DGLowMemory::LTSLoadFieldsAndResidues(const uint fKSave, const uint lKSave) {
}

void
DGLowMemory::addRHSToRes(const uint e1, const uint e2, const double rka,
		const double dt) {
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
