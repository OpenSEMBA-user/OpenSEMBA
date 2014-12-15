/*
 * SmbData.cpp
 *
 *  Created on: Mar 28, 2014
 *      Author: luis
 */

#include "SmbData.h"

SmbData::SmbData() {
    layers = NULL;
	mesh = NULL;
	gData = NULL;
	pMGroup = NULL;
	emSources = NULL;
	outputRequests = NULL;
	ofParams = NULL;
}

SmbData::~SmbData() {
}

SmbData&
SmbData::operator=(const SmbData& rhs) {
	if (this == &rhs) {
		return *this;
	}
	layers = new LayerGroup(*rhs.layers);
	mesh = new MeshVolume(*rhs.mesh);
	gData = new GlobalProblemData(*rhs.gData);
	pMGroup = new PhysicalModelGroup(*rhs.pMGroup);
	emSources = new EMSourceGroup(*rhs.emSources);
	outputRequests = new OutputRequestGroup(*rhs.outputRequests);
	ofParams = new OpenFOAMParameters(*rhs.ofParams);
	return *this;
}

void
SmbData::ignorePMLs() {
	const uint pmlId = pMGroup->getPML()->getId();
	const uint vacuumId = pMGroup->getVacuum()->getId();
	const vector<uint> pmlIds = mesh->getIdsWithMaterialId(pmlId);
	mesh->setMaterialIds(pmlIds, vacuumId);
}

void
SmbData::detectAndAssignPMLRegions() {
	pMGroup->detectAndAssignPMLRegions(mesh);
}
//
//SmbData::SmbData(const SmbData* smb_, const MeshOpenfoam& ofMesh) {
//	*this = smb_;
//
//	// PERFORMANCE: This function can be improved by reading only the
//	// relevant elements and coordinates from the openFoam data.
//	mesh->elem = reassignMaterials(mesh->elem, ofMesh.boundaries_);
//
//	outputRequests = reassignOutputRequests(outputRequests, ofMesh.boundaries_);
//	emSources = reassignEMSources(emSources, ofMesh.boundaries_);
//
//	mesh->elem = mesh->elem.removeElementsWithMatId(0);
//}
//
//ElementsGroup
//SmbData::reassignMaterials(
// const ElementsGroup& elem,
// const vector<OpenfoamSurface>& b) const {
//	const uint nB = b.size();
//	uint lastMatId = 0;
//	uint id;
//	// Assigns materials.
//	for (uint i = 0; i < nB; i++) {
//		if (b[i].isMaterial()) {
//			id = b[i].getMaterialId();
//			if (id > lastMatId) {
//				lastMatId = id;
//			}
//			uint nElem = b[i].getFaces();
//			uint startFace = b[i].getStartFace();
//			for (uint e = 0; e < nElem; e++) {
//				uint bId = startFace + e;
//				elem.getPtrToId(bId)->setMatId(id);
//			}
//		}
//	}
//
//	// For each volumic material creates hex and assigns mat to hex.
//	vector<uint> volMatId = smb_->pMGroup->getVolumicMatIds();
//	for (uint i = 0; i < volMatId.size(); i++) {
//		res.setVolumeElementsFromClosedSurfacesWithMatId(volMatId[i]);
//	}
//
//
//	// Assigns outputRequests.
//	for (uint i = 0; i < nB; i++) {
//		if (b[i].isOutputRequest()) {
//			id = lastMatId + b[i].getOutputRequestId();
//			uint nElem = b[i].getFaces();
//			uint startFace = b[i].getStartFace();
//			for (uint e = 0; e < nElem; e++) {
//				uint bId = startFace + e;
//				elem.getPtrToId(bId)->setMatId(id);
//			}
//		}
//	}
//	// Assigns Sources on surfaces.
//	lastMatId = id;
//	for (uint i = 0; i < nB; i++) {
//		if (b[i].isEMSource()) {
//			id = lastMatId + b[i].getEMSourceId();
//			uint nElem = b[i].getFaces();
//			uint startFace = b[i].getStartFace();
//			for (uint e = 0; e < nElem; e++) {
//				uint bId = startFace + e;
//				elem.getPtrToId(bId)->setMatId(id);
//			}
//		}
//	}
//	return elem;
//}

void
SmbData::printInfo() const {
	cout << " --- SEMBA data --- " << endl;
	if (layers != NULL) {
	    layers->printInfo();
	} else {
	    cout << "No info about layers." << endl;
	}
	if (mesh != NULL) {
		mesh->printInfo();
	} else {
		cout << "No info about mesh." << endl;
	}
	if (gData != NULL) {
	    gData->printInfo();
	} else {
	    cout << "No info about global data." << endl;
	}
	if (pMGroup != NULL) {
	    pMGroup->printInfo();
	} else {
	    cout << "No info about physical models." << endl;
	}
	if (emSources != NULL) {
	    emSources->printInfo();
	} else {
	    cout << "No info about electromagnetic sources." << endl;
	}
	if (outputRequests != NULL) {
	    outputRequests->printInfo();
	} else {
	    cout << "No info about output requests." << endl;
	}
	if (ofParams != NULL) {
	    ofParams->printInfo();
	} else {
	    cout << "No info about openFoam parameters." << endl;
	}

}

void
SmbData::applyGeometricScalingFactor() {
	mesh->applyGeometricScalingFactor(gData->scalingFactor);
	outputRequests->applyGeometricScalingFactor(gData->scalingFactor);
	emSources->applyGeometricScalingFactor(gData->scalingFactor);
	gData->applyGeometricScalingFactor(gData->scalingFactor);
	gData->scalingFactor = (double) 1.0;
}
