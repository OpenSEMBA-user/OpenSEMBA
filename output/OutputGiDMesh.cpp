/*
 *  Created on: Aug 23, 2012
 *      Author: luis
 */
#ifndef OUTPUTGIDMESH_H_
#include "OutputGiDMesh.h"
#endif

OutputGiDMesh::OutputGiDMesh() {
	initDefault();
}

OutputGiDMesh::OutputGiDMesh(
 const string& outputName_,
 const string& projectName_,
 const string& projectDir_) {
	initDefault();
	outputName = outputName_;
	projectName = projectName_;
	projectDir = projectDir_;
	//deleteExistentOutputFiles();
	openGiDFiles();
}

OutputGiDMesh::~OutputGiDMesh() {
	if (GiDType == GiD_PostAscii) {
		GiD_ClosePostMeshFile();
	}
	GiD_ClosePostResultFile();
}

void
OutputGiDMesh::write(const MeshVolume& mesh) {
	if (mesh.isRectilinear()) {
		writeRectilinearMesh(mesh);
	} else {
		cerr << "ERROR @ OutputGiDMesh" << endl;
		cerr << "Outputs are not implemented for this type of mesh." << endl;
	}
}

void
OutputGiDMesh::write(const NFDEData& mesh) {

#	warning "Not implemented"

}

void
OutputGiDMesh::flush() const {
	GiD_FlushPostFile();
}
void
OutputGiDMesh::openGiDFiles() {
	assert(GiDType == GiD_PostAscii || GiDType == GiD_PostBinary);
	string mshFileName = projectDir + outputName + ".post.msh";
	string resFileName = projectDir + outputName + ".post.res";
	char *auxChar;
	auxChar = new char[mshFileName.length() + 1];
	switch (GiDType) {
	case GiD_PostAscii:
		strcpy(auxChar, mshFileName.c_str());
		GiD_OpenPostMeshFile(auxChar, GiDType);
		strcpy(auxChar, resFileName.c_str());
		GiD_OpenPostResultFile(auxChar, GiDType);
		break;
	case GiD_PostBinary:
		strcpy(auxChar, resFileName.c_str());
		GiD_OpenPostResultFile(auxChar, GiDType);
		break;
	default:
		cerr << "ERROR @ GiDOutput::openFiles()" << endl;
		cerr << "Unreckognized GiD Type." << endl;
		assert(false);
	}
	//
	delete [] auxChar;
}

void
OutputGiDMesh::writeQuad4Mesh(
 const string& name,
 const MeshVolume& mesh) {
	static const int nV = 4;
	char* tName;
	tName = new char[name.length() + 1];
	strcpy(tName, name.c_str());
	GiD_BeginMesh(tName, GiD_3D, GiD_Quadrilateral, nV);
	delete tName;
	// Writes coordinates.
	GiD_BeginCoordinates();
	coordCounterPreStart = coordCounter;
	CartesianVector<double, 3> pos;
	for (uint i = 0; i < mesh.v.size(); i++) {
		pos = mesh.v(i)->pos();
		GiD_WriteCoordinates(++coordCounter, pos(0), pos(1), pos(2));
	}
	GiD_EndCoordinates();
	// Writes element connectivities.
	GiD_BeginElements();
	int nId[nV + 1];
	static const uint GiDOrder[4] = { 0, 1, 2, 3 };
	for (uint e = 0; e < mesh.elem.quad4.size(); e++) {
		const Quad4* quad = &mesh.elem.quad4[e];
		for (int i = 0; i < nV; i++) {
			nId[i] = quad->getVertex(GiDOrder[i])->id + 1
					+ coordCounterPreStart;
		}
		nId[nV] = quad->getMatId();
		GiD_WriteElementMat(++elemCounter, nId);
	}
	GiD_EndElements();
	GiD_EndMesh();
}

void
OutputGiDMesh::writeHex8Mesh(
 const string& name,
 const MeshVolume& mesh) {
	static const int nV = 8;
	char* tName;
	tName = new char[name.length() + 1];
	strcpy(tName, name.c_str());
	GiD_BeginMesh(tName, GiD_3D, GiD_Hexahedra, nV);
	delete tName;
	// Writes coordinates.
	GiD_BeginCoordinates();
	coordCounterPreStart = coordCounter;
	CartesianVector<double, 3> pos;
	for (uint i = 0; i < mesh.v.size(); i++) {
		pos = *mesh.v(i);
		GiD_WriteCoordinates(++coordCounter, pos(0), pos(1), pos(2));
	}
	GiD_EndCoordinates();
	// Writes element connectivities.
	GiD_BeginElements();
	int nId[nV + 1];
	static const uint GiDOrder[8] = { 0, 1, 2, 3, 4, 5, 6, 7};
	for (uint e = 0; e < mesh.elem.hex8.size(); e++) {
		const Hex8* hex = &mesh.elem.hex8[e];
		for (int i = 0; i < nV; i++) {
			nId[i] = hex->getVertex(GiDOrder[i])->id + 1
					+ coordCounterPreStart;
		}
		nId[nV] = hex->getMatId();
		GiD_WriteElementMat(++elemCounter, nId);
	}
	GiD_EndElements();
	GiD_EndMesh();
}

void
OutputGiDMesh::writeRectilinearMesh(const MeshVolume& mesh) {
	string name = "Material";
	writeQuad4Mesh(name, mesh);
	writeHex8Mesh(name, mesh);
}

void
OutputGiDMesh::deleteExistentOutputFiles() const {
	string fileName = projectDir + projectName;
	string cmd;
#ifdef __unix__
	cmd = "rm " + fileName + ".post.msh";
	system(cmd.c_str());
	cmd = "rm " + fileName + ".post.res";
	system(cmd.c_str());
#else
	#error "Function not defined for non-unix systems"
#endif
}

void
OutputGiDMesh::initDefault() {
#ifdef GIDOUTPUT_ASCII_OUTPUT
	GiDType = GiD_PostAscii;
#else
	GiDType = GiD_PostBinary;
#endif
	coordCounter = 0;
	elemCounter = 0;
	coordCounterPreStart = 0;
}
